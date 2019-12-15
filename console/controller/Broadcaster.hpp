#pragma once
#include <iostream>
#include <thread>

namespace Controller {

class Broadcaster {
public:
    Broadcaster()
        : m_thread { std::thread(&Broadcaster::start, this) }
    {
        m_thread.detach();
    }

    ~Broadcaster() { m_running = false; }

private:
    std::thread m_thread;
    bool m_running { true };

    void start();
};

}
