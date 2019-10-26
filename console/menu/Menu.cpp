#include "Menu.hpp"

namespace Menu {

void Menu::update(std::shared_ptr<ControllerState> state) { }

void Menu::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);

    RlDrawText("Main Menu", 100, 100, 12, MAROON);

    EndTextureMode();
}

RenderTexture2D Menu::framebuffer() { return m_framebuffer; }

}
