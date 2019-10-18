#pragma once
#include <atomic>
#include <boost/asio/io_context.hpp>
#include <thread>

class Controller {
private:
    boost::asio::io_context ctx;
    std::thread thread;
    void worker();

public:
    struct State {
        std::atomic_bool left;
        std::atomic_bool forward;
        std::atomic_bool right;
    };

    std::shared_ptr<State> state;

    Controller()
        : state(std::make_shared<State>())
        , thread { std::thread(&Controller::worker, this) }
    {
    }

    ~Controller()
    {
        ctx.stop();
        thread.join();
    }
};
