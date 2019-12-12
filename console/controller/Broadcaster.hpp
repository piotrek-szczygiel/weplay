#pragma once
#include <thread>

namespace Controller {

class Broadcaster {
public:
    Broadcaster()
        : m_thread { std::thread(&Broadcaster::start, this) }
    {
    }

    ~Broadcaster()
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
