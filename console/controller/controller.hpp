#pragma once
#include <thread>

class Controller {
private:
    std::thread thread;
    void worker();

public:
    Controller() { thread = std::thread(&Controller::worker, this); }
};