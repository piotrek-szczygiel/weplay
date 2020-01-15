#include "Console.hpp"
#include "pong/Pong.hpp"
#include "starship/Starship.hpp"
#include "tetris/Tetris.hpp"
#include <algorithm>
#include <raylib.h>

void Console::run()
{
    while (!WindowShouldClose()) {

        auto state = m_current_state->state_change();
        switch (state) {
        case StateChange::Menu: {
            m_current_state = std::make_unique<Menu::Menu>();
            break;
        }
        case StateChange::Starship: {
            m_current_state = std::make_unique<Starship::Starship>();
            break;
        }
        case StateChange::Tetris: {
            m_current_state = std::make_unique<Tetris::Tetris>();
            break;
        }
        case StateChange::Pong: {
            m_current_state = std::make_unique<Pong::Pong>();
            break;
        }
        case StateChange::None: {
            break;
        }
        }

        const auto& controllers = m_server.get_controllers().get_states();

        m_current_state->update(controllers);

        BeginDrawing();
        ClearBackground(BLACK);

        m_current_state->draw();

        if (m_show_fps) {
            DrawFPS(10, 10);
        }

        EndDrawing();
    }
}
