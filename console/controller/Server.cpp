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

        if (result.ignore || result.data == "@") {
            spdlog::trace("Ignored packet from {}: {}", result.repr, result.data);
            continue;
        }

        spdlog::debug("Received from {}: {}", result.repr, result.data);
    }
}

}