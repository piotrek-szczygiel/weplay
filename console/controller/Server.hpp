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

private:
    std::thread m_thread;
    bool m_running { true };

    void start();
};

}
