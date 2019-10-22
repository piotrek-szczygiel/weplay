#pragma once
#include "Controller.hpp"
#include "TcpSession.hpp"
#include <boost/asio.hpp>
#include <memory>

namespace ba = boost::asio;
using ba::ip::tcp;

class TcpServer {
public:
    TcpServer(ba::io_context& io_context, short port, std::shared_ptr<ControllerState> state)
        : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
        , m_controller_state(std::move(state))
    {
        do_accept();
    }

private:
    tcp::acceptor m_acceptor;
    std::shared_ptr<ControllerState> m_controller_state;

    void do_accept();
};
