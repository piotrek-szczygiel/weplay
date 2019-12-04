#pragma once
#include <array>
#include <atomic>
#include <boost/asio/io_context.hpp>
#include <thread>

struct ControllerState {
    std::array<bool, 16> buttons {};
    int16_t yaw {};
    int16_t pitch {};
    int16_t roll {};
};

class Controller {
public:
    std::shared_ptr<ControllerState> state;

    Controller()
        : state(std::make_shared<ControllerState>())
        , m_controller_thread { std::thread(&Controller::controller_worker, this) }
        , m_broadcaster_thread { std::thread(&Controller::broadcaster_worker, this) }
    {
    }

    ~Controller()
    {
        m_ctx.stop();
        m_controller_thread.join();
        m_broadcaster_thread.join();
    }

private:
    boost::asio::io_context m_ctx;
    std::thread m_controller_thread;
    std::thread m_broadcaster_thread;

    void controller_worker();
    void broadcaster_worker();
};
