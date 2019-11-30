#include "Menu.hpp"
#include <boost/format.hpp>
#include <ctime>

using boost::str, boost::format;

namespace Menu {

void Menu::update(std::shared_ptr<ControllerState> state)
{
    if (IsKeyPressed(KEY_F1) || state->buttons[8]) {
        m_state_change = StateChange::Menu;
    } else if (IsKeyPressed(KEY_F2) || state->buttons[0]) {
        m_state_change = StateChange::Starship;
    } else if (IsKeyPressed(KEY_F3) || state->buttons[1]) {
        m_state_change = StateChange::Tetris;
    } else if (IsKeyPressed(KEY_F4) || state->buttons[2]) {
        m_state_change = StateChange::Pong;
    } else if (IsKeyPressed(KEY_LEFT)) {
        m_game_index = m_game_index == 0 ? (GAMES - 1) : m_game_index - 1;
    } else if (IsKeyPressed(KEY_RIGHT)) {
        m_game_index = (m_game_index + 1) % GAMES;
    }

    m_yaw = state->yaw;
    m_pitch = state->pitch;
    m_roll = state->roll;
    m_buttons = state->buttons;

    m_seconds += GetFrameTime();
    SetShaderValue(m_shader, m_seconds_loc, &m_seconds, UNIFORM_FLOAT);
}

void Menu::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);

    BeginShaderMode(m_shader);
    DrawTexture(m_bg, 0, 0, WHITE);
    EndShaderMode();

    RlDrawText(str(format("Yaw: %d") % m_yaw).c_str(), 10, 100, 16, RAYWHITE);
    RlDrawText(str(format("Pitch: %d") % m_pitch).c_str(), 10, 130, 16, RAYWHITE);
    RlDrawText(str(format("Roll: %d") % m_roll).c_str(), 10, 160, 16, RAYWHITE);

    draw_game_name();
    draw_game_image();

    for (size_t i = 0; i < m_buttons.size(); ++i) {
        if (m_buttons[i]) {
            RlDrawText(str(format("Button %d pressed") % i).c_str(), 10,
                200 + 30 * static_cast<int>(i), 16, RAYWHITE);
        }
    }

    char time[16];

    const auto raw_time = std::time(nullptr);
    const auto time_info = localtime(&raw_time);
    strftime(time, sizeof(time), "%H:%M", time_info);
    RlDrawText(time, 980, 10, 16, RAYWHITE);

    EndTextureMode();
}

RenderTexture2D Menu::framebuffer() { return m_framebuffer; }

void Menu::draw_game_name() {
    m_game_name_position = {m_width / 2 - MeasureText(m_games_names[m_game_index].c_str(), 20) / 2};

    RlDrawText(str(format("%s") % m_games_names[m_game_index]).c_str(), m_game_name_position, 600, 20, RAYWHITE);
}
void Menu::draw_game_image() {
    DrawTexture(m_games_images[m_game_index], m_width / 2 - 400 / 2, m_height / 2 - 400 / 2, RAYWHITE);
}

}
