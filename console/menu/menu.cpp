#include "menu.hpp"

void Menu::update(std::shared_ptr<Controller::State> state) { }

void Menu::draw()
{
    BeginTextureMode(framebuffer);
    ClearBackground(BLACK);

    RlDrawText("Main Menu", 100, 100, 12, MAROON);

    EndTextureMode();
}

RenderTexture2D Menu::get_framebuffer()
{
    return framebuffer;
}
