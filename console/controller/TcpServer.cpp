#include "TcpServer.hpp"

void TcpServer::do_accept()
{
    m_acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::make_shared<TcpSession>(std::move(socket), m_state)->read();
        }

        do_accept();
    });
}
