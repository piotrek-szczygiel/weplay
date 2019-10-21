#include "Menu.hpp"

namespace Menu {

void Menu::update(std::shared_ptr<Controller::State> state) { }

void Menu::draw()
{
    BeginTextureMode(framebuffer_);
    ClearBackground(BLACK);

    RlDrawText("Main Menu", 100, 100, 12, MAROON);

    EndTextureMode();
}

RenderTexture2D Menu::getFramebuffer()
{
    return framebuffer_;
}

}
