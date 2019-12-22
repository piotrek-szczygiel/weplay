#include "Broadcaster.hpp"
#include "../Config.hpp"
#include "../Logging.hpp"
#include "Network.hpp"

using namespace std::chrono_literals;

namespace Controller {

void Broadcaster::start()
{
    const uint16_t port = Config::integer("network", "broadcast_port", 1985);

    spdlog::debug("Obtaining local network addresses");
    auto addresses = get_local_addresses();
    if (addresses.empty()) {
        spdlog::error("No network interfaces to broadcast on");
        return;
    } else {
        spdlog::debug("Local network addresses:");
        for (const auto& a : addresses) {
            spdlog::debug("\t{}\t{}", int_to_ip(a.address), int_to_ip(a.mask));
        }
    }

    std::vector<BroadcastSocket> sockets {};

    for (auto& address : addresses) {
        BroadcastSocket socket { address, port };
        if (socket.initialize()) {
            sockets.push_back(socket);
            spdlog::info("Broadcasting on {}", socket.info());
        } else {
            spdlog::warn("Unable to initialize broadcasting socket {}", socket.info());
        }
    }

    if (sockets.empty()) {
        spdlog::error("Unable to create any broadcasting socket");
        return;
    }

    while (m_running) {
        for (auto& socket : sockets) {
            if (!socket.send("weplay")) {
                spdlog::error("Unable to send broadcast on {}: {}", socket.info(), last_error());
            }
        }

        std::this_thread::sleep_for(1s);
    }
}
}
