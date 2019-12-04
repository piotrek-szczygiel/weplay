#include "Controller.hpp"
#include "TcpServer.hpp"
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>

void Controller::controller_worker()
{
    BOOST_LOG_TRIVIAL(info) << "Starting controller worker thread";

    try {
        TcpServer server { m_ctx, 1984, state };
        m_ctx.run();
    } catch (std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Exception while running TCP server: " << e.what();
    }
}

void on_send(const boost::system::error_code& ec, size_t bytes)
{
    if (ec) {
        BOOST_LOG_TRIVIAL(warning) << "Broadcaster error: " << ec.message();
    } else {
        BOOST_LOG_TRIVIAL(info) << "Broadcaster sent " << bytes << " bytes";
    }
}

void Controller::broadcaster_worker()
{
    BOOST_LOG_TRIVIAL(info) << "Starting broadcaster thread";

    std::vector<ba::ip::udp::socket> sockets {};
    std::vector<std::string> addresses {};

    // Open one socket for every network interface
    ba::io_service io_service;
    ba::ip::tcp::resolver resolver(io_service);
    ba::ip::tcp::resolver::query query(ba::ip::host_name(), "");
    auto it = resolver.resolve(query);
    while (it != ba::ip::tcp::resolver::iterator()) {
        auto addr = (it++)->endpoint().address();
        if (addr.is_v4()) {
            ba::ip::udp::socket socket { m_ctx };
            boost::system::error_code ec;
            socket.open(ba::ip::udp::v4(), ec);
            if (ec) {
                BOOST_LOG_TRIVIAL(error) << "Unable to create broadcast socket on "
                                         << addr.to_string() << ": " << ec.message();
            } else {
                socket.set_option(ba::ip::udp::socket::reuse_address(true));
                socket.set_option(ba::socket_base::broadcast(true));
                socket.bind(ba::ip::udp::endpoint(addr, 0));

                sockets.push_back(std::move(socket));
                addresses.push_back(addr.to_string());
            }
        }
    }

    if (sockets.size() < 1) {
        BOOST_LOG_TRIVIAL(warning) << "No viable interfaces found to broadcast";
        return;
    }

    for (auto& address : addresses) {
        BOOST_LOG_TRIVIAL(info) << "Broadcasting on " << address << "...";
    }

    // Broadcast interface ip address every second
    while (true) {
        for (size_t i = 0; i < sockets.size(); ++i) {
            sockets[i].async_send_to(ba::buffer(addresses[i].c_str(), addresses[i].size()),
                ba::ip::udp::endpoint(ba::ip::address_v4::broadcast(), 2137),
                boost::bind(
                    &on_send, ba::placeholders::error, ba::placeholders::bytes_transferred));
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
