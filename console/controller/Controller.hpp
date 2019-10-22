#pragma once
#include <atomic>
#include <boost/asio/io_context.hpp>
#include <thread>

struct ControllerState {
    std::atomic_bool left;
    std::atomic_bool forward;
    std::atomic_bool right;
};

class Controller {
public:
    std::shared_ptr<ControllerState> state;

    Controller()
        : state(std::make_shared<ControllerState>())
        , m_thread { std::thread(&Controller::worker, this) }
    {
    }

    ~Controller()
    {
        m_ctx.stop();
        m_thread.join();
    }

private:
    boost::asio::io_context m_ctx;
    std::thread m_thread;

    void worker();
};
