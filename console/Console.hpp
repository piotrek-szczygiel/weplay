#pragma once
#include "State.hpp"
#include <memory>

class Console {
private:
    std::unique_ptr<State> currentState;

public:
    Console();
    void Initialize();
    void Run();
};
