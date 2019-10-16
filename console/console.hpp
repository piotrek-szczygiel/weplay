#pragma once
#include "controller/controller.hpp"
#include "state.hpp"
#include <memory>

class Console {
private:
    std::unique_ptr<State> current_state;
    // Controller controller;

public:
    Console();
    void init();
    void run();
};
