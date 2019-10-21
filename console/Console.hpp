#pragma once
#include "State.hpp"
#include "controller/Controller.hpp"
#include "menu/Menu.hpp"
#include <memory>

class Console {
public:
    Console()
        : currentState_ { std::make_unique<Menu::Menu>() }
    {
    }

    void run();

private:
    std::unique_ptr<State> currentState_;
    Controller controller_;
};
