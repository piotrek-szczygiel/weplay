#include "../Config.hpp"
#include "Broadcaster.hpp"
#include "Server.hpp"
#include <cluon.hpp>
#include <spdlog/spdlog.h>

using namespace std::literals::chrono_literals;

namespace Controller {

void Broadcaster::start()
{
    std::vector<std::unique_ptr<cluon::UDPSender>> senders {};

    uint16_t port = Config::integer("network", "port", 1984);
    std::stringstream ss { Config::string("network", "broadcast", "127.0.0.1") };

    std::string address;
    while (std::getline(ss, address, ' ')) {
        spdlog::info("Creating broadcaster on {}:{}", address, port);
        senders.push_back(std::make_unique<cluon::UDPSender>(address, port));
    }

    int tick = 0;
    while (m_running) {
        ++tick;

        if ((tick %= 10) == 0) {
            for (auto& sender : senders) {
                sender->send("@");
            }
        }

        std::this_thread::sleep_for(100ms);
    }

    spdlog::info("Stopping broadcasters");
}

void Server::start()
{
    std::string address = Config::string("network", "address", "0.0.0.0");
    uint16_t port = Config::integer("network", "port", 1984);
    spdlog::info("Starting server on {}:{}", address, port);

    cluon::UDPReceiver receiver {
        address,
        port,
        [](std::string&& data, std::string&& sender,
            std::chrono::system_clock::time_point&& ts) noexcept {
            if (data == "@") {
                return;
            }

            const auto timestamp = std::chrono::system_clock::to_time_t(ts);
            spdlog::info(
                "Received {} bytes from {} at {} '{}'", data.size(), sender, timestamp, data);
        },
    };

    while (m_running) {
        m_running = receiver.isRunning();
        std::this_thread::sleep_for(100ms);
    }

    spdlog::info("Stopping server");
}

}
