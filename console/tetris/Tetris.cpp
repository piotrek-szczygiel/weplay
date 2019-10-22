#include "Tetris.hpp"
#include "Piece.hpp"
#include <iostream>

namespace Tetris {

void Tetris::update(std::shared_ptr<ControllerState> state)
{
    std::vector<Action> p1 {};

    if (IsKeyDown(KEY_LEFT)) {
        p1.push_back(Action::MOVE_LEFT);
    } else if (IsKeyDown(KEY_RIGHT)) {
        p1.push_back(Action::MOVE_RIGHT);
    }

    if (IsKeyDown(KEY_DOWN)) {
        p1.push_back(Action::MOVE_DOWN);
    }

    if (IsKeyDown(KEY_SPACE)) {
        p1.push_back(Action::HARD_DROP);
    } else if (IsKeyDown(KEY_LEFT_SHIFT)) {
        p1.push_back(Action::SOFT_DROP);
    }

    if (IsKeyDown(KEY_Z)) {
        p1.push_back(Action::ROTATE_LEFT);
    } else if (IsKeyDown(KEY_X)) {
        p1.push_back(Action::ROTATE_RIGHT);
    }

    float dt = GetFrameTime();
    m_player_1.update(dt, p1);
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
