#include "Broadcaster.hpp"
#include "../Config.hpp"
#include <cluon.hpp>
#include <inih.h>
#include <memory>
#include <sstream>

using namespace std::literals::chrono_literals;

namespace Controller {

void Broadcaster::start()
{
    std::vector<std::unique_ptr<cluon::UDPSender>> senders {};

    std::stringstream ss(Config::string("network", "broadcast", "127.0.0.1"));
    std::string address;
    while (std::getline(ss, address, ' ')) {
        std::cout << "Creating broadcaster on " << address << "\n";
        senders.push_back(std::make_unique<cluon::UDPSender>(address, 1984));
    }

    while (m_running) {
        for (auto& sender : senders) {
            sender->send("raspberry-console");
        }

        std::this_thread::sleep_for(1s);
    }
}

}
