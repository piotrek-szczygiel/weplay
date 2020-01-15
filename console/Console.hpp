#pragma once
#include "Config.hpp"
#include "State.hpp"
#include "controller/Broadcaster.hpp"
#include "controller/Server.hpp"
#include "menu/Menu.hpp"
#include <memory>

class Console {
public:
    Console()
        : m_current_state { std::make_unique<Menu::Menu>() }
    {
        m_show_fps = Config::boolean("window", "show_fps", false);
    }

    void run();

private:
    std::unique_ptr<State> m_current_state;

    Controller::Broadcaster m_broadcaster;
    Controller::Server m_server;

    bool m_show_fps;
};
