#include "TcpServer.hpp"
#include <boost/log/trivial.hpp>

void TcpServer::do_accept()
{
    m_acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            auto session = std::make_shared<TcpSession>(std::move(socket), m_state);
            m_sessions.push_back(session);
            session->start();
        }

        do_accept();
    });

    m_sessions.erase(std::remove_if(m_sessions.begin(), m_sessions.end(),
                         [](auto session) { return !session || session->closed(); }),
        m_sessions.end());
}
