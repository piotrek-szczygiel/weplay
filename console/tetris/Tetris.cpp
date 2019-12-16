#include "Tetris.hpp"
#include "Context.hpp"
#include "DrawBlock.hpp"
#include "Piece.hpp"

namespace Tetris {

void Tetris::update(const std::vector<ControllerState>& controllers)
{
    std::array<std::vector<Action>, 2> actions {};

    for (size_t player = 0; player < 2; ++player) {
        const auto& state = controllers[player];

        if (state.buttons[8]) {
            m_state_change = StateChange::Menu;
            return;
        }

        if (state.buttons[0]) {
            actions[player].push_back(Action::MOVE_LEFT);
        } else if (state.buttons[3]) {
            actions[player].push_back(Action::MOVE_RIGHT);
        }

        if (state.buttons[1]) {
            actions[player].push_back(Action::MOVE_DOWN);
        }

        if (state.buttons[5]) {
            actions[player].push_back(Action::HARD_DROP);
        } else if (state.buttons[2]) {
            actions[player].push_back(Action::SOFT_DROP);
        }

        if (state.buttons[4]) {
            actions[player].push_back(Action::ROTATE_LEFT);
        } else if (state.buttons[9]) {
            actions[player].push_back(Action::ROTATE_RIGHT);
        }

        if (state.buttons[6]) {
            actions[player].push_back(Action::HOLD);
        }
    }

    if (m_player_1.enough_lines() || m_player_2.enough_lines()) {
        m_player_1.reset_lines();
        m_player_2.reset_lines();

        m_player_1.increase_level();
        m_player_2.increase_level();
    }

    float dt = GetFrameTime();
    m_player_1.update(dt, actions[0]);
    m_player_2.update(dt, actions[1]);
}

void Tetris::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);

    m_player_1.draw(57, 57, true);
    m_player_2.draw(m_width - 56 - WIDTH * TILE_SIZE, 57, false);

    DrawTexture(Context::instance().texture_ui, 0, 0, WHITE);
    EndTextureMode();
}

RenderTexture2D Tetris::framebuffer() { return m_framebuffer; }
}
