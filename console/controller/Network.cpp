#include "Network.hpp"
#include <algorithm>
#include <spdlog/spdlog.h>

#ifdef linux
#include <ifaddrs.h>
#include <libdevmapper.h>
#include <libnet.h>
#include <netdb.h>
#include <netinet/in.h>

std::vector<LocalAddress> get_local_addresses()
{
    std::vector<LocalAddress> result {};

    ifaddrs* if_addr;
    if (getifaddrs(&if_addr) == -1) {
        spdlog::error("Unable to get interfaces information");
        return result;
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
                spdlog::debug("Ignoring loopback interface");
                continue;
            }

            result.push_back({ inet_addr(host), inet_addr(mask) });
        }
    }

    freeifaddrs(if_addr);
    return result;
}

std::string BroadcastSocket::info() const
{
    char addr[16];
    inet_ntop(AF_INET, &m_address, addr, sizeof(addr));
    return std::string(addr) + ":" + std::to_string(m_port);
}

bool BroadcastSocket::initialize()
{
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (m_socket < 0) {
        spdlog::error("Unable to create broadcasting socket");
        return false;
    }

    int yes = 1;
    if (setsockopt(m_socket, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(yes)) < 0) {
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
    int sent = sendto(m_socket, data.c_str(), data.size(), 0,
        reinterpret_cast<const sockaddr*>(&m_addr), sizeof(m_addr));

    return sent == 1;
}

bool ServerSocket::bind()
{
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (m_socket < 0) {
        spdlog::error("Unable to create UDP socket: {}", strerror(errno));
        return false;
    }

    timeval tv {};
    tv.tv_sec = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = INADDR_ANY;
    m_addr.sin_port = htons(m_port);

    if (::bind(m_socket, reinterpret_cast<const sockaddr*>(&m_addr), sizeof(m_addr)) < 0) {
        spdlog::error("Unable to create UDP server on port {}: {}", m_port, strerror(errno));
        return false;
    }

    m_ignore = get_local_addresses();
    return true;
}

ReceiveResult ServerSocket::receive()
{
    ReceiveResult result {};

    sockaddr_in addr {};
    int addr_len = sizeof(addr);
    char buffer[1024];

    int size = recvfrom(m_socket, buffer, sizeof(buffer) - 1, MSG_WAITALL,
        reinterpret_cast<sockaddr*>(&addr), reinterpret_cast<socklen_t*>(&addr_len));

    if (size <= 0) {
        result.success = false;
        return result;
    }

    buffer[size] = '\0';

    result.success = true;
    result.address = addr.sin_addr.s_addr;
    result.port = addr.sin_port;
    result.data = std::string(buffer);

    char addr_str[16];
    inet_ntop(AF_INET, &addr.sin_addr.s_addr, addr_str, sizeof(addr_str));
    result.repr = std::string(addr_str) + ":" + std::to_string(addr.sin_port);

    // Ignore local addresses
    for (auto& i : m_ignore) {
        if (i.address == result.address) {
            result.ignore = true;
            break;
        }
    }

    return result;
}

#endif // linux
