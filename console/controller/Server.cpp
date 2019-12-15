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

    // Remove inactive controllers after 5 seconds
    Timestamp last_check {};
    auto timeout = 5s;

    while (m_running) {
        auto now = std::chrono::system_clock::now();
        if (now - last_check > timeout) {
            auto removed = m_controllers.remove_timed_out(now, timeout);
            if (removed > 0) {
                spdlog::warn("Removed {} timed out controller{}", removed, removed > 1 ? "s" : "");
            }
        }

        auto result = server.receive();
        if (!result.success) {
            spdlog::error("Error while receiving: {}", strerror(errno));
            continue;
        }

        auto str = std::string { result.data, result.data + result.size };

        if (result.ignore) {
            spdlog::trace("Ignored packet from {} ({}B): {}", result.repr, result.size, str);
            continue;
        }

        if (str == "ping") {
            server.send(result.addr, "pong");
            spdlog::trace("Ping from {}", result.repr);
            continue;
        }

        if (result.size == 8) {
            ControllerState state {
                .connected = true,
                .last_update = std::chrono::system_clock::now(),
            };

            for (uint8_t i = 0; i < 16; ++i) {
                uint8_t byte = i / 8;
                uint8_t bit = i % 8;

                state.buttons[i] = !(static_cast<uint8_t>(result.data[byte]) & (1u << bit));
            }

            state.yaw = result.data[2];
            state.yaw |= result.data[3] << 8u; // NOLINT(hicpp-signed-bitwise)

            state.pitch = result.data[4];
            state.pitch |= result.data[5] << 8u; // NOLINT(hicpp-signed-bitwise)

            state.roll = result.data[6];
            state.roll |= result.data[7] << 8u; // NOLINT(hicpp-signed-bitwise)

            if (m_controllers.set_state(result.repr, state)) {
                spdlog::info("New controller connected: {}", result.repr);
            }

            continue;
        }

        spdlog::warn("Unknown packet from {} ({}B): {}", result.repr, result.size, result.data);
    }
}

}