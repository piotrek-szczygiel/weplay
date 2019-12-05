#pragma once
#include "Controller.hpp"
#include <boost/asio.hpp>
#include <memory>

class TcpSession : public std::enable_shared_from_this<TcpSession> {
public:
    explicit TcpSession(ba::ip::tcp::socket socket, std::shared_ptr<AllControllersState> state)
        : m_socket(std::move(socket))
        , m_deadline(m_socket.get_executor())
        , m_data {}
        , m_state(std::move(state))
        , m_valid_controller { false }
    {
    }

    void start();

private:
    enum class NextRead {
        Mode,
        Buttons,
        Mpu6050,
    };

    NextRead m_next_read { NextRead::Mode };
    size_t m_read_size { 1 };

    ba::ip::tcp::socket m_socket;
    std::array<uint8_t, 16> m_data;

    ba::steady_timer m_deadline;

    std::shared_ptr<AllControllersState> m_state;
    size_t m_id { 0 };

    bool m_valid_controller;

    void read();
    void check_deadline();

    bool register_controller();
    void unregister_controller();
};
