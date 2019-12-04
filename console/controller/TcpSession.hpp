#pragma once
#include "Controller.hpp"
#include <boost/asio.hpp>
#include <memory>

class TcpSession : public std::enable_shared_from_this<TcpSession> {
public:
    explicit TcpSession(boost::asio::ip::tcp::socket socket, std::shared_ptr<ControllerState> state)
        : m_socket(std::move(socket))
        , m_data {}
        , m_controller_state(std::move(state))
        , m_valid_controller { false }
    {
    }

    void read();

private:
    enum class NextRead {
        Mode,
        Buttons,
        Mpu6050,
    };

    NextRead m_next_read { NextRead::Mode };
    size_t m_read_size { 1 };

    boost::asio::ip::tcp::socket m_socket;
    std::array<uint8_t, 16> m_data;

    std::shared_ptr<ControllerState> m_controller_state;

    bool m_valid_controller;
};
