#include <asio.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

using asio::ip::tcp;

class Tcp_Session : public std::enable_shared_from_this<Tcp_Session> {
public:
    explicit Tcp_Session(tcp::socket socket)
        : socket(std::move(socket))
    {
    }

    void start() { do_read(); }

private:
    void do_read()
    {
        auto self = shared_from_this();
        socket.async_read_some(
            asio::buffer(buffer, max_length), [this, self](std::error_code error, std::size_t length) {
                if (!error) {
                    do_write(length);
                }
            });
    }

    void do_write(std::size_t length)
    {
        auto self(shared_from_this());
        asio::async_write(
            socket, asio::buffer(buffer, length), [this, self](std::error_code error, std::size_t /*length*/) {
                if (!error) {
                    do_read();
                }
            });
    }

    static const size_t max_length = 1024;

    tcp::socket socket;
    std::vector<uint8_t> buffer;
};

class Tcp_Server {
public:
    Tcp_Server(asio::io_context& io_context, int16_t port)
        : acceptor(io_context, tcp::endpoint(tcp::v4(), port))
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor.async_accept([this](std::error_code error, tcp::socket socket) {
            if (!error) {
                std::make_shared<Tcp_Session>(std::move(socket))->start();
            }

            do_accept();
        });
    }

    tcp::acceptor acceptor;
};

void start_controller()
{
    try {
        asio::io_context io_context;
        Tcp_Server s(io_context, 1984);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";
    }
}