#include "Server.hpp"
#include "Network.hpp"
#include <spdlog/spdlog.h>

namespace Controller {

void Server::start()
{
    spdlog::info("Starting server");
    ServerSocket server { 1984 };
    if (!server.bind()) {
        spdlog::error("Unable to start server");
        return;
    }

    while (m_running) {
        auto result = server.receive();
        if (!result.success) {
            spdlog::error("Error while receiving: {}", strerror(errno));
            continue;
        }

        auto str = std::string { result.data, result.data + result.size };

        if (result.ignore) {
            spdlog::trace("Ignored packet from {}: {}", result.repr, result.data);
            continue;
        }

        if (str == "ping") {
            server.send(result.addr, "pong");
            spdlog::trace("Ping from {}", result.repr);
            continue;
        }

        if (result.size == 8) {
            for (uint8_t i = 0; i < 16; ++i) {
                uint8_t byte = i / 8;
                uint8_t bit = i % 8;

                buttons[i] = !(static_cast<uint8_t>(result.data[byte]) & (1u << bit));
            }

            yaw = result.data[2];
            yaw |= result.data[3] << 8u;

            pitch = result.data[4];
            pitch |= result.data[5] << 8u;

            roll = result.data[6];
            roll |= result.data[7] << 8u;

            spdlog::trace("{}\t{}:{}:{}", buttons[0], yaw, pitch, roll);
            continue;
        }

        spdlog::warn("Unknown packet received from {} ({} bytes): {}", result.repr, result.size,
            result.data);
    }
}

}