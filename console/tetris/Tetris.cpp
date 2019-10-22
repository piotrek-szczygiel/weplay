#include "Tetris.hpp"
#include "Piece.hpp"
#include <iostream>

namespace Tetris {

void Tetris::update(std::shared_ptr<ControllerState> state)
{
    std::vector<Action> player_1_actions {};

    if (IsKeyDown(KEY_LEFT)) {
        player_1_actions.push_back(Action::MOVE_LEFT);
    } else if (IsKeyDown(KEY_RIGHT)) {
        player_1_actions.push_back(Action::MOVE_RIGHT);
    }

    if (IsKeyDown(KEY_DOWN)) {
        player_1_actions.push_back(Action::MOVE_DOWN);
    }

    if (IsKeyDown(KEY_SPACE)) {
        player_1_actions.push_back(Action::HARD_DROP);
    } else if (IsKeyDown(KEY_LEFT_SHIFT)) {
        player_1_actions.push_back(Action::SOFT_DROP);
    }

    if (IsKeyDown(KEY_Z)) {
        player_1_actions.push_back(Action::ROTATE_COUNTER_CLOCKWISE);
    } else if (IsKeyDown(KEY_X)) {
        player_1_actions.push_back(Action::ROTATE_CLOCKWISE);
    }

    float dt = GetFrameTime();
    m_player_1.update(dt, std::move(player_1_actions));
}

void Tetris::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);

    m_player_1.draw(30, 30);
    m_player_2.draw(m_width - 30 - WIDTH * BLOCK_SIZE, 30);

    EndTextureMode();
}

RenderTexture2D Tetris::framebuffer() { return m_framebuffer; }
}
