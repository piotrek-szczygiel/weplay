#pragma once
#include "controller/controller.hpp"
#include "menu/menu.hpp"
#include "state.hpp"
#include <memory>

class Console {
private:
    std::unique_ptr<State> current_state;
    Controller controller;

public:
    Console()
        : current_state { std::make_unique<Menu>() }
    {
    }

    void run();
};
