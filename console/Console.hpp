#pragma once
#include "State.hpp"
#include "controller/Broadcaster.hpp"
#include "menu/Menu.hpp"
#include <memory>

class Console {
public:
    Console()
        : m_current_state { std::make_unique<Menu::Menu>() }
    {
    }

    void run();

private:
    std::unique_ptr<State> m_current_state;

    Controller::Broadcaster m_broadcaster;
};
