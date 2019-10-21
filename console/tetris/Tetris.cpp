#include "Tetris.hpp"
#include "Piece.hpp"
#include <iostream>

namespace Tetris {

void Tetris::update(std::shared_ptr<Controller::State> state)
{
    auto collision = std::bind(&Matrix::collision, matrix_, std::placeholders::_1);
    if (IsKeyPressed(KEY_LEFT)) {
        piece_.move(-1, 0, collision);
    } else if (IsKeyPressed(KEY_RIGHT)) {
        piece_.move(1, 0, collision);
    }

    if (IsKeyPressed(KEY_DOWN)) {
        piece_.move(0, 1, collision);
    }

    if (IsKeyPressed(KEY_UP)) {
        piece_.move(0, -1, collision);
    }

    if (IsKeyPressed(KEY_SPACE)) {
        matrix_.lock(piece_);
        piece_ = Piece { SHAPE_J };
    }

    if (IsKeyPressed(KEY_Z)) {
        piece_.rotate(false, collision);
    } else if (IsKeyPressed(KEY_X)) {
        piece_.rotate(true, collision);
    }
}

void Tetris::draw()
{
    BeginTextureMode(framebuffer_);
    ClearBackground(BLACK);

    matrix_.draw(30, 30);
    piece_.draw(30, 30);

    // RlDrawText("Tetris", 100, 100, 32, GREEN);

    EndTextureMode();
}

RenderTexture2D Tetris::getFramebuffer()
{
    return framebuffer_;
}

}
