#pragma once
#include <boost/asio/io_context.hpp>
#include <boost/lockfree/queue.hpp>
#include <thread>

enum Controller_Key {
    C_KEY_LEFT,
    C_KEY_MIDDLE,
    C_KEY_RIGHT,
};

struct Event_Key {
    Controller_Key key;
    bool down;
};

class Controller {
private:
    boost::asio::io_context ctx;
    std::thread thread;
    void worker();

public:
    boost::lockfree::queue<Event_Key> queue;

    Controller()
        : queue(128)
    {
        thread = std::thread(&Controller::worker, this);
    }
    ~Controller()
    {
        ctx.stop();
        thread.join();
    }
};
