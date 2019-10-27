#pragma once
#include <array>
#include <atomic>
#include <boost/asio/io_context.hpp>
#include <thread>

struct ControllerState {
    std::array<std::atomic_bool, 8> buttons {};
    std::atomic_int16_t yaw {};
    std::atomic_int16_t pitch {};
    std::atomic_int16_t roll {};
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
