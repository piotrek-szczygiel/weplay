#pragma once
#include <atomic>
#include <boost/asio/io_context.hpp>
#include <thread>

class Controller {
public:
    struct State {
        std::atomic_bool left;
        std::atomic_bool forward;
        std::atomic_bool right;
    };

    std::shared_ptr<State> state;

    Controller()
        : state(std::make_shared<State>())
        , thread_ { std::thread(&Controller::worker, this) }
    {
    }

    ~Controller()
    {
        ctx_.stop();
        thread_.join();
    }

private:
    boost::asio::io_context ctx_;
    std::thread thread_;

    void worker();
};
