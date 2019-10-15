#pragma once
#include "state.hpp"
#include <memory>

class Console {
private:
    std::unique_ptr<State> current_state;

public:
    Console();
    void init();
    void run();
};
