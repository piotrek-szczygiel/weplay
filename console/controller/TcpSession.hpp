#pragma once
#include "Controller.hpp"
#include <boost/asio.hpp>
#include <memory>

class TcpSession : public std::enable_shared_from_this<TcpSession> {
public:
    explicit TcpSession(boost::asio::ip::tcp::socket socket, std::shared_ptr<Controller::State> state)
        : socket_(std::move(socket))
        , controllerState_(std::move(state))
        , data_ {}
        , validController_ { false }
    {
    }

    void start();

private:
    static const size_t max_length_ { 4 };

    boost::asio::ip::tcp::socket socket_;
    std::array<uint8_t, max_length_> data_;

    std::shared_ptr<Controller::State> controllerState_;

    bool validController_;

    void doRead();
};
