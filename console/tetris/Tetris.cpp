#include "Tetris.hpp"
#include "DrawBlock.hpp"
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

    if (IsKeyDown(KEY_C)) {
        p1.push_back(Action::HOLD);
    }

    if (IsKeyPressed(KEY_L)) {
        m_player_1.increase_level();
        m_player_2.increase_level();
    }

    float dt = GetFrameTime();
    m_player_1.update(dt, p1);
    m_player_2.update(dt, {});
}

void Tetris::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);

    m_player_1.draw(35, 50);
    m_player_2.draw(m_width - 35 - WIDTH * TILE_SIZE, 50);

    EndTextureMode();
}

RenderTexture2D Tetris::framebuffer() { return m_framebuffer; }
}
