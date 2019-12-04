#pragma once
#include "Controller.hpp"
#include <boost/asio.hpp>
#include <memory>

class TcpSession : public std::enable_shared_from_this<TcpSession> {
public:
    explicit TcpSession(
        boost::asio::ip::tcp::socket socket, std::shared_ptr<AllControllersState> state)
        : m_socket(std::move(socket))
        , m_data {}
        , m_state(std::move(state))
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

    std::shared_ptr<AllControllersState> m_state;
    size_t m_id { 0 };

    bool m_valid_controller;

    bool register_controller();
    void unregister_controller();
};
