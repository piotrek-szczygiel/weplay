#include "tetris.hpp"

void Tetris::update(std::shared_ptr<Controller::State> state) { }

void Tetris::draw()
{
    BeginTextureMode(framebuffer);
    ClearBackground(BLACK);

    RlDrawText("Tetris", 100, 100, 32, GREEN);

    EndTextureMode();
}

RenderTexture2D Tetris::get_framebuffer()
{
    return framebuffer;
}
