#include "Tetris.hpp"
#include "Piece.hpp"
#include <iostream>

namespace Tetris {

void Tetris::update(std::shared_ptr<Controller::State> state)
{
    if (IsKeyPressed(KEY_A)) {
        matrix_.lock(piece_);
    }

    if (IsKeyPressed(KEY_B)) {
        for (int y = 0; y < HEIGHT + VANISH; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                std::cout << matrix_.get(x, y) << " ";
            }
            std::cout << std::endl;
        }
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
