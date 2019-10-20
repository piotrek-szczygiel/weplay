#include "TcpServer.hpp"

void TcpServer::doAccept()
{
    acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
        if (!ec) {
            std::make_shared<TcpSession>(std::move(socket), controllerState_)->start();
        }

        doAccept();
    });
}
