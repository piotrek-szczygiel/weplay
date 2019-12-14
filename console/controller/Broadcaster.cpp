#include "Broadcaster.hpp"
#include <spdlog/spdlog.h>
#include <string>
#include <utility>
#include <vector>

using namespace std::chrono_literals;

#ifdef linux
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <sys/socket.h>

std::vector<uint32_t> get_local_addresses()
{
    std::vector<uint32_t> result {};

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

            auto broadcast = inet_addr(host) | (~inet_addr(mask));
            result.push_back(broadcast);
        }
    }

    freeifaddrs(if_addr);
    return result;
}

class BroadcastSocket {
public:
    BroadcastSocket(uint32_t address, uint16_t port)
        : m_address { address }
        , m_port { port }
    {
    }

    std::string info() const
    {
        char addr[16];
        inet_ntop(AF_INET, &m_address, addr, sizeof(addr));
        return std::string(addr) + ":" + std::to_string(m_port);
    }

    bool initialize()
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

    bool send(const std::string& data)
    {
        int sent = sendto(m_socket, data.c_str(), data.size(), 0,
            reinterpret_cast<sockaddr*>(&m_addr), sizeof(m_addr));

        return sent == 1;
    }

private:
    uint32_t m_address;
    uint16_t m_port;

    int m_socket {};
    sockaddr_in m_addr {};
};

#endif // linux

#ifdef _WIN32

#endif // _WIN32

namespace Controller {

void Broadcaster::start()
{
    const uint16_t port = 1984;

    spdlog::debug("Obtaining local network addresses");
    auto addresses = get_local_addresses();
    if (addresses.empty()) {
        spdlog::error("No network interfaces to broadcast on");
        return;
    }

    std::vector<BroadcastSocket> sockets {};

    for (auto& address : addresses) {
        BroadcastSocket socket { address, port };
        if (socket.initialize()) {
            sockets.push_back(socket);
            spdlog::info("Broadcasting on {}", socket.info());
        } else {
            spdlog::warn("Unable to initialize broadcasting socket {}:{}", address, port);
        }
    }

    while (m_running) {
        for (auto& socket : sockets) {
            if (!socket.send("@")) {
                spdlog::error("Unable to send broadcast on {}: {}", socket.info(), strerror(errno));
            }
        }

        std::this_thread::sleep_for(1s);
    }
}

}
