#pragma once
#include <boost/asio/io_context.hpp>
#include <thread>

class Controller {
private:
    boost::asio::io_context ctx;
    std::thread thread;
    void worker();

public:
    Controller() { thread = std::thread(&Controller::worker, this); }
    ~Controller()
    {
        ctx.stop();
        thread.join();
    }
};