#include "Tetris.hpp"
#include "Piece.hpp"

namespace Tetris {

void Tetris::update(std::shared_ptr<Controller::State> state) { }

void Tetris::draw()
{
    BeginTextureMode(framebuffer_);
    ClearBackground(BLACK);

    RlDrawText("Tetris", 100, 100, 32, GREEN);

    EndTextureMode();
}

RenderTexture2D Tetris::getFramebuffer()
{
    return framebuffer_;
}

}
