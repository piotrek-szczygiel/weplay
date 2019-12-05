#pragma once
#include "Controller.hpp"
#include "TcpSession.hpp"
#include <boost/asio.hpp>
#include <memory>

namespace ba = boost::asio;
using ba::ip::tcp;

class TcpServer {
public:
    TcpServer(ba::io_context& io_context, short port, std::shared_ptr<AllControllersState> state)
        : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
        , m_state(std::move(state))
    {
        do_accept();
    }

private:
    tcp::acceptor m_acceptor;
    std::shared_ptr<AllControllersState> m_state;

    std::vector<std::shared_ptr<TcpSession>> m_sessions {};

    void do_accept();
};
