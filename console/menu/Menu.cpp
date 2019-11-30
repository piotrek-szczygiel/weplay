#include "Menu.hpp"
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <ctime>

using boost::str, boost::format;

namespace Menu {

float tween(float value, float x);
float tween_x2(float value, float x);

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
    } else if (IsKeyPressed(KEY_LEFT) && m_animation_state == NONE) {
        m_last_game_index = m_game_index;
        m_game_index = m_game_index == 0 ? (GAMES - 1) : m_game_index - 1;
        m_animation_state = PLAYING_LEFT;
    } else if (IsKeyPressed(KEY_RIGHT) && m_animation_state == NONE) {
        m_last_game_index = m_game_index;
        m_game_index = (m_game_index + 1) % GAMES;
        m_animation_state = PLAYING_RIGHT;
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
    draw_game_image(GetFrameTime());

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

void Menu::draw_game_name()
{
    m_game_name_position
        = { m_width / 2 - MeasureText(m_games_names[m_game_index].c_str(), 20) / 2 };

    RlDrawText(str(format("%s") % m_games_names[m_game_index]).c_str(), m_game_name_position, 600,
        20, RAYWHITE);
}
void Menu::draw_game_image(float dt)
{
    int posX { m_width + 400 };
    float tween_value = static_cast<float>(m_width) / 2;

    if (m_animation_state == PLAYING_LEFT) {
        int x2 = m_width / 2 - 400 / 2 - static_cast<int>(tween(tween_value, m_animation_timer));

        if (m_animation_timer >= 0.5F) {
            int x = m_width / 2
                + static_cast<int>(tween(tween_value, 1.0F - (m_animation_timer - 0.5F)));
            posX = x - 400 / 2;
        } else if (m_animation_timer >= 1.0F) {
            x2 = -400;
        }

        m_animation_timer += dt;
        if (m_animation_timer >= 1.5F) {
            m_animation_state = ENDING;
        }

        DrawTexture(m_games_images[m_last_game_index], x2, m_height / 2 - 400 / 2, RAYWHITE);
    } else if (m_animation_state == PLAYING_RIGHT) {
        int x2 = m_width / 2 - 400 / 2 + static_cast<int>(tween(tween_value, m_animation_timer));

        if (m_animation_timer >= 0.5F) {
            int x = m_width / 2
                - static_cast<int>(tween(tween_value, 1.0F - (m_animation_timer - 0.5F)));
            posX = x - 400 / 2;
        } else if (m_animation_timer >= 1.0F) {
            x2 = -400;
        }

        m_animation_timer += dt;
        if (m_animation_timer >= 1.5F) {
            m_animation_state = ENDING;
        }

        DrawTexture(m_games_images[m_last_game_index], x2, m_height / 2 - 400 / 2, RAYWHITE);
    } else if (m_animation_state == ENDING) {
        posX = m_width / 2 - 400 / 2;
        m_animation_state = NONE;
        m_animation_timer = 0.0F;
    } else if (m_animation_state == NONE) {
        posX = m_width / 2 - 400 / 2;
    }
    DrawTexture(m_games_images[m_game_index], posX, m_height / 2 - 400 / 2, RAYWHITE);
}

float tween(float value, float x) { return value * (x * x * x); }
float tween_x2(float value, float x) { return value * (x); }

}
