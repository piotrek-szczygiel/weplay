#pragma once
#include "state.hpp"
#include <easylogging++.h>
#include <memory>

class Console {
private:
    std::unique_ptr<State> current_state;
    el::Logger* log;

public:
    Console();
    void init();
    void run();
};
