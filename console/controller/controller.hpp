#pragma once
#include <boost/asio/io_context.hpp>
#include <thread>

class Controller {
private:
    boost::asio::io_context io_context;
    std::thread thread;
    void worker();

public:
    Controller() { thread = std::thread(&Controller::worker, this); }
    ~Controller()
    {
        io_context.stop();
        thread.join();
    }
};