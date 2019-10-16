#include "controller.hpp"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

namespace ba = boost::asio;
using ba::ip::tcp;

class Tcp_Session : public std::enable_shared_from_this<Tcp_Session> {
private:
    static const size_t max_length = 1024;

    tcp::socket socket;
    boost::array<uint8_t, max_length> data;

    void do_read()
    {
        auto self = shared_from_this();
        socket.async_read_some(ba::buffer(data, max_length),
            [this, self](boost::system::error_code ec, std::size_t length) {
                auto address = socket.remote_endpoint().address().to_string();

                if (!ec) {
                    BOOST_LOG_TRIVIAL(info)
                        << "Received message " << length << " bytes from " << address;

                    do_read();
                } else {
                    BOOST_LOG_TRIVIAL(error)
                        << "Error while receiving data from " << address << ": " << ec.message();
                }
            });
    }

public:
    explicit Tcp_Session(tcp::socket socket)
        : socket(std::move(socket))
        , data {}
    {
    }

    void start()
    {
        BOOST_LOG_TRIVIAL(info) << "Started new session with "
                                << socket.remote_endpoint().address().to_string();

        do_read();
    }
};

class Tcp_Server {
private:
    tcp::acceptor acceptor;

    void do_accept()
    {
        acceptor.async_accept([this](boost::system::error_code ec, tcp::socket socket) {
            if (!ec) {
                std::make_shared<Tcp_Session>(std::move(socket))->start();
            }

            do_accept();
        });
    }

public:
    Tcp_Server(ba::io_context& io_context, short port)
        : acceptor(io_context, tcp::endpoint(tcp::v4(), port))
    {
        do_accept();
    }
};

void Controller::worker()
{
    BOOST_LOG_TRIVIAL(info) << "Starting controller worker";
    try {
        Tcp_Server s(ctx, 1984);
        ctx.run();
    } catch (std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << "Exception while running TCP server: " << e.what();
    }
}