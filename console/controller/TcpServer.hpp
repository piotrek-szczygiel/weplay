#pragma once
#include "Controller.hpp"
#include "TcpSession.hpp"
#include <boost/asio.hpp>
#include <memory>

namespace ba = boost::asio;
using ba::ip::tcp;

class TcpServer {
public:
    TcpServer(ba::io_context& io_context, short port, std::shared_ptr<Controller::State> state)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
        , controllerState_(std::move(state))
    {
        doAccept();
    }

private:
    tcp::acceptor acceptor_;
    std::shared_ptr<Controller::State> controllerState_;

    void doAccept();
};
