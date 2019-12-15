#pragma once
#include <iostream>
#include <thread>

namespace Controller {

class Server {
public:
    Server()
        : m_thread { std::thread(&Server::start, this) }
    {
    }

    ~Server()
    {
        m_running = false;
        m_thread.join();
    }

    std::array<bool, 16> buttons {};
    int16_t yaw;
    int16_t pitch;
    int16_t roll;

private:
    std::thread m_thread;
    bool m_running { true };

    void start();
};

}
