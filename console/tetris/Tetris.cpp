#include "Tetris.hpp"
#include "Piece.hpp"
#include <iostream>

namespace Tetris {

void Tetris::update(std::shared_ptr<Controller::State> state)
{
    if (IsKeyPressed(KEY_LEFT))
        player1.action(Action::MOVE_LEFT);
    if (IsKeyPressed(KEY_RIGHT))
        player1.action(Action::MOVE_RIGHT);
    if (IsKeyPressed(KEY_DOWN))
        player1.action(Action::SOFT_DROP);
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_SPACE))
        player1.action(Action::HARD_DROP);
    if (IsKeyPressed(KEY_Z))
        player1.action(Action::ROTATE_COUNTER_CLOCKWISE);
    if (IsKeyPressed(KEY_X))
        player1.action(Action::ROTATE_CLOCKWISE);

    float dt = GetFrameTime();
    player1.update(dt);
}

void Tetris::draw()
{
    BeginTextureMode(framebuffer_);
    ClearBackground(BLACK);

    player1.draw(30, 30);
    player2.draw(width_ - 30 - WIDTH * BLOCK_SIZE, 30);

    EndTextureMode();
}

RenderTexture2D Tetris::getFramebuffer()
{
    return framebuffer_;
}

}
