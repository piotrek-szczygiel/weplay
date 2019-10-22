#include "Tetris.hpp"
#include "Piece.hpp"
#include <iostream>

namespace Tetris {

void Tetris::update(std::shared_ptr<Controller::State> state)
{
    std::vector<Action> player1Actions {};

    if (IsKeyDown(KEY_LEFT)) {
        player1Actions.push_back(Action::MOVE_LEFT);
    } else if (IsKeyDown(KEY_RIGHT)) {
        player1Actions.push_back(Action::MOVE_RIGHT);
    }

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_SPACE)) {
        player1Actions.push_back(Action::HARD_DROP);
    } else if (IsKeyDown(KEY_DOWN)) {
        player1Actions.push_back(Action::SOFT_DROP);
    }

    if (IsKeyDown(KEY_Z)) {
        player1Actions.push_back(Action::ROTATE_COUNTER_CLOCKWISE);
    } else if (IsKeyDown(KEY_X)) {
        player1Actions.push_back(Action::ROTATE_CLOCKWISE);
    }

    float dt = GetFrameTime();
    player1.update(dt, std::move(player1Actions));
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
