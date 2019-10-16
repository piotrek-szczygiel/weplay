#include "controller.hpp"
#include <asio.hpp>
#include <cstdlib>
#include <easylogging++.h>
#include <iostream>
#include <memory>
#include <utility>

using asio::ip::tcp;

class Tcp_Session : public std::enable_shared_from_this<Tcp_Session> {
private:
    static const size_t max_length = 1024;

    tcp::socket socket;
    std::array<uint8_t, max_length> buffer;

    void read()
    {
        auto self = shared_from_this();
        socket.async_read_some(
            asio::buffer(buffer, max_length), [this, self](std::error_code error, std::size_t length) {
                if (!error) {
                    write(length);
                } else {
                    CLOG(ERROR, "controller")
                        << "Error while reading from " << socket.remote_endpoint().address().to_string() << ": "
                        << error.message();
                }
            });
    }

    void write(std::size_t length)
    {
        auto self(shared_from_this());
        asio::async_write(
            socket, asio::buffer(buffer, length), [this, self](std::error_code error, std::size_t /* length */) {
                if (!error) {
                    read();
                } else {
                    CLOG(ERROR, "controller")
                        << "Error while writing to " << socket.remote_endpoint().address().to_string() << ": "
                        << error.message();
                }
            });
    }

public:
    explicit Tcp_Session(tcp::socket socket)
        : socket(std::move(socket))
        , buffer {}
    {
    }

    void start()
    {
        CLOG(INFO, "controller") << "Started new session with " << socket.remote_endpoint().address().to_string();
        read();
    }
};

class Tcp_Server {
private:
    tcp::acceptor acceptor;

    void accept()
    {
        CLOG(INFO, "controller") << "Awaiting connection";
        acceptor.async_accept([this](std::error_code error, tcp::socket socket) {
            if (!error) {
                std::make_shared<Tcp_Session>(std::move(socket))->start();
            } else {
                CLOG(ERROR, "controller") << "Unable to accept connection: " << error.message();
            }

            accept();
        });
    }

public:
    Tcp_Server(asio::io_context& io_context, int16_t port)
        : acceptor(io_context, tcp::endpoint(tcp::v4(), port))
    {
        accept();
    }
};

void Controller::worker()
{
    try {
        asio::io_context io_context;
        Tcp_Server s(io_context, 1984);
        io_context.run();
    } catch (std::exception& e) {
        CLOG(ERROR, "controller") << "Exception occured: " << e.what();
    }
}