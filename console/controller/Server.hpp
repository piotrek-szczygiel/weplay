#pragma once
#include "Controllers.hpp"
#include <iostream>
#include <thread>

namespace Controller {

class Server {
public:
    Server()
        : m_thread { std::thread(&Server::start, this) }
    {
        m_thread.detach();
    }

    ~Server() { m_running = false; }

    Controllers& get_controllers() { return m_controllers; }

private:
    std::thread m_thread;
    bool m_running { true };

    Controllers m_controllers {};

    void start();
};

}
