#include "Tetris.hpp"
#include "Context.hpp"
#include "DrawBlock.hpp"
#include "Piece.hpp"
#include <iostream>

namespace Tetris {

void Tetris::update(std::shared_ptr<ControllerState> state)
{
    std::vector<Action> p1 {};
    std::vector<Action> p2 {};

    if (IsKeyDown(KEY_LEFT) || state->roll.load() < -20 || state->buttons[0].load()) {
        p1.push_back(Action::MOVE_LEFT);
    } else if (IsKeyDown(KEY_RIGHT) || state->roll.load() > 20 || state->buttons[1].load()) {
        p1.push_back(Action::MOVE_RIGHT);
    }

    if (IsKeyDown(KEY_DOWN) || state->pitch.load() > 20 || state->buttons[2].load()) {
        p1.push_back(Action::MOVE_DOWN);
    }

    if (IsKeyDown(KEY_SPACE) || state->buttons[4].load()) {
        p1.push_back(Action::HARD_DROP);
    } else if (IsKeyDown(KEY_LEFT_SHIFT) || state->buttons[5].load()) {
        p1.push_back(Action::SOFT_DROP);
    }

    if (IsKeyDown(KEY_Z) || state->buttons[6].load()) {
        p1.push_back(Action::ROTATE_LEFT);
    } else if (IsKeyDown(KEY_X) || state->buttons[7].load()) {
        p1.push_back(Action::ROTATE_RIGHT);
    }

    if (IsKeyDown(KEY_C) || state->buttons[3].load()) {
        p1.push_back(Action::HOLD);
    }

    if (IsKeyPressed(KEY_L)) {
        m_player_1.increase_level();
        m_player_2.increase_level();
    }

    if (m_player_1.enough_lines() || m_player_2.enough_lines()) {
        m_player_1.reset_lines();
        m_player_2.reset_lines();

        m_player_1.increase_level();
        m_player_2.increase_level();
    }

    float dt = GetFrameTime();
    m_player_1.update(dt, p1);
    m_player_2.update(dt, p1);
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
