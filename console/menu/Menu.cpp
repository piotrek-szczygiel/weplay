#include "Menu.hpp"
#include <boost/format.hpp>

using boost::str, boost::format;

namespace Menu {

void Menu::update(std::shared_ptr<ControllerState> state)
{
    if (IsKeyPressed(KEY_Q) || state->buttons[8]) {
        m_exit = true;
        return;
    }

    m_yaw = state->yaw;
    m_pitch = state->pitch;
    m_roll = state->roll;
    m_buttons = state->buttons;
}

void Menu::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);

    RlDrawText(str(format("Yaw: %d") % m_yaw).c_str(), 10, 100, 16, RAYWHITE);
    RlDrawText(str(format("Pitch: %d") % m_pitch).c_str(), 10, 130, 16, RAYWHITE);
    RlDrawText(str(format("Roll: %d") % m_roll).c_str(), 10, 160, 16, RAYWHITE);

    for (size_t i = 0; i < m_buttons.size(); ++i) {
        if (m_buttons[i]) {
            RlDrawText(
                str(format("Button %d pressed") % i).c_str(), 10, 200 + 30 * i, 16, RAYWHITE);
        }
    }

    EndTextureMode();
}

RenderTexture2D Menu::framebuffer() { return m_framebuffer; }

}
