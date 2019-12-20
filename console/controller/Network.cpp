#include "Network.hpp"
#include "../Logging.hpp"
#include <algorithm>

#ifdef _WIN32
#include "../Windows.hpp"
#else
#include <ifaddrs.h>
#include <libdevmapper.h>
#include <libnet.h>
#include <netdb.h>
#endif

#ifdef _WIN32
std::string last_error()
{
    char* s = nullptr;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&s, 0, NULL);

    std::string message(s);
    LocalFree(s);

    return message;
}
#else
std::string last_error() { return std::string(strerror(errno)); }
#endif

std::string int_to_ip(uint32_t ip)
{
    in_addr addr;
    addr.s_addr = ip;
    return inet_ntoa(addr);
}

#ifdef _WIN32
std::vector<LocalAddress> get_local_addresses()
{
    std::vector<LocalAddress> result {};

    PMIB_IPADDRTABLE address_table = (MIB_IPADDRTABLE*)malloc(sizeof(MIB_IPADDRTABLE));
    if (address_table == nullptr) {
        spdlog::error("Memory allocation failed for IP address table");
        return {};
    }

    DWORD table_size = 0;
    if (GetIpAddrTable(address_table, &table_size, 0) == ERROR_INSUFFICIENT_BUFFER) {
        free(address_table);
        address_table = (MIB_IPADDRTABLE*)malloc(table_size);
    }

    if (address_table == nullptr) {
        spdlog::error("Memory allocation failed for GetIpAddrTable");
        return {};
    }

    DWORD get_table_result = GetIpAddrTable(address_table, &table_size, 0);
    if (get_table_result != NO_ERROR) {
        spdlog::error("GetIpAddrTable failed with error {}", get_table_result);
        return {};
    }

    IN_ADDR ip_address {};
    DWORD entries = address_table->dwNumEntries;

    for (unsigned i = 0; i < entries; i++) {
        uint32_t address = address_table->table[i].dwAddr;
        uint32_t mask = address_table->table[i].dwMask;

        if (address == htonl(INADDR_LOOPBACK)) {
            spdlog::trace("Ignoring loopback interface");
            continue;
        }

        result.push_back({ address, mask });
    }

    if (address_table) {
        free(address_table);
    }

    return result;
}
#else
std::vector<LocalAddress> get_local_addresses()
{
    std::vector<LocalAddress> result {};

    ifaddrs* if_addr;
    if (getifaddrs(&if_addr) == -1) {
        spdlog::error("Unable to get interfaces information");
        return {};
    }

    int n = 0;
    for (ifaddrs* ifa = if_addr; ifa != nullptr; ifa = ifa->ifa_next, n++) {
        if (ifa->ifa_addr == nullptr)
            continue;

        int family = ifa->ifa_addr->sa_family;

        if (family == AF_INET) {
            char host[NI_MAXHOST];
            if (getnameinfo(ifa->ifa_addr, sizeof(sockaddr_in), host, NI_MAXHOST, nullptr, 0,
                    NI_NUMERICHOST)
                != 0) {
                spdlog::error("Unable to query interface address");
                continue;
            }

            char mask[NI_MAXHOST];
            if (getnameinfo(ifa->ifa_netmask, sizeof(sockaddr_in), mask, NI_MAXHOST, nullptr, 0,
                    NI_NUMERICHOST)
                != 0) {
                spdlog::error("Unable to query interface mask");
                continue;
            }

            if (strcmp(host, "127.0.0.1") == 0) {
                spdlog::trace("Ignoring loopback interface");
                continue;
            }

            spdlog::trace("Queried local interface: {} {}", host, mask);
            result.push_back({ inet_addr(host), inet_addr(mask) });
        }
    }

    freeifaddrs(if_addr);
    return result;
}
#endif

std::string BroadcastSocket::info() const
{
    return int_to_ip(m_address) + ":" + std::to_string(m_port);
}

bool BroadcastSocket::initialize()
{
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (m_socket == INVALID_SOCKET) {
        spdlog::error("Unable to create broadcasting socket");
        return false;
    }

    int yes = 1;
    if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, (char*)&yes, sizeof(yes)) < 0) {
        spdlog::error("Unable to set broadcast option on a socket");
        return false;
    }

    m_addr.sin_family = AF_INET;
    m_addr.sin_port = htons(m_port);
    m_addr.sin_addr.s_addr = m_address;

    return true;
}

bool BroadcastSocket::send(const std::string& data)
{
    int sent = sendto(m_socket, data.c_str(), data.size(), 0, (sockaddr*)&m_addr, sizeof(m_addr));

    return sent == data.size();
}

bool ServerSocket::bind()
{
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socket == INVALID_SOCKET) {
        spdlog::error("Unable to create UDP socket: {}", last_error());
        return false;
    }

#ifdef _WIN32
    DWORD timeout = 1000;
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
#else
    timeval tv {};
    tv.tv_sec = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
#endif

    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port = htons(m_port);

    if (::bind(m_socket, (sockaddr*)&m_addr, sizeof(m_addr)) == INVALID_SOCKET) {
        spdlog::error("Unable to create UDP server on port {}: {}", m_port, last_error());
        return false;
    }

    return true;
}

ReceiveResult ServerSocket::receive()
{
    ReceiveResult result {};

    sockaddr_in addr {};
    int addr_len = sizeof(addr);
    uint8_t buffer[1024];

#ifdef _WIN32
    int size
        = recvfrom(m_socket, (char*)buffer, sizeof(buffer) - 1, 0, (sockaddr*)&addr, &addr_len);

    if (WSAGetLastError() == WSAETIMEDOUT) {
        result.timedout = true;
    }
#else
    int size = recvfrom(
        m_socket, buffer, sizeof(buffer) - 1, MSG_WAITALL, (sockaddr*)&addr, (socklen_t*)&addr_len);

    if (errno = EAGAIN) {
        result.timedout = true;
    }
#endif

    if (size <= 0) {
        result.success = false;
        return result;
    }

    result.success = true;
    result.addr = addr;
    result.data = buffer;
    result.size = size;

    result.repr = int_to_ip(addr.sin_addr.s_addr) + ":" + std::to_string(addr.sin_port);

    return result;
}

bool ServerSocket::send(sockaddr_in addr, const std::string& data)
{
    int sent = sendto(m_socket, data.c_str(), data.size(), 0, (sockaddr*)&addr, sizeof(addr));
    return sent == data.size();
}
