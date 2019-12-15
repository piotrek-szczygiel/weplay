#include "Menu.hpp"
#include <ctime>

namespace Menu {

float tween(float value, float x);

void Menu::update()
{
    // const auto& s1 = all_states->controllers[0];
    // const auto& s2 = all_states->controllers[1];

    // if ((IsKeyPressed(KEY_RIGHT) || s1.buttons[3] || s2.buttons[3])
    //    && m_animation_state == AnimationState::NONE) {
    //    m_last_game_index = m_game_index;
    //    m_game_index = m_game_index == 0 ? (GAMES - 1) : m_game_index - 1;
    //    m_animation_state = AnimationState::PLAYING_RIGHT;
    //} else if ((IsKeyPressed(KEY_LEFT) || s1.buttons[0] || s2.buttons[0])
    //    && m_animation_state == AnimationState::NONE) {
    //    m_last_game_index = m_game_index;
    //    m_game_index = (m_game_index + 1) % GAMES;
    //    m_animation_state = AnimationState::PLAYING_LEFT;
    //} else if (IsKeyPressed(KEY_ENTER) || s1.buttons[5] || s2.buttons[5]) {
    //    m_state_change = m_games_states[m_game_index];
    //}

    // m_connected = all_states->connected_num;

    // m_yaw = s1.yaw;
    // m_pitch = s1.pitch;
    // m_roll = s1.roll;
    // m_buttons = s1.buttons;
}

void Menu::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);

    DrawTextureEx(m_background[m_background_frame], { 0.0F, 0.0F }, 0.0F, 2.0F, WHITE);
    m_background_timer += GetFrameTime();
    if (m_background_timer >= 0.07F) {
        m_background_timer -= 0.07F;
        m_background_frame += 1;

        m_background_frame %= m_background.size();
    }

    DrawText(TextFormat("Connected controllers: %d", m_connected), 10, 70, 16, RAYWHITE);
    DrawText(TextFormat("Yaw: %d", m_yaw), 10, 100, 16, RAYWHITE);
    DrawText(TextFormat("Pitch: %d", m_pitch), 10, 130, 16, RAYWHITE);
    DrawText(TextFormat("Roll: %d", m_roll), 10, 160, 16, RAYWHITE);

    draw_game_name(64);
    draw_game_image(GetFrameTime() * SLIDE_SPEED);

    for (size_t i = 0; i < m_buttons.size(); ++i) {
        if (m_buttons[i]) {
            DrawText(TextFormat("Button %d pressed", i), 10, 200 + 30 * static_cast<int>(i), 16,
                RAYWHITE);
        }
    }

    char time[16];

    const auto raw_time = std::time(nullptr);
    const auto time_info = localtime(&raw_time);
    strftime(time, sizeof(time), "%H:%M", time_info);
    DrawText(time, 980, 10, 16, RAYWHITE);

    EndTextureMode();
}

void Menu::draw_game_name(int font_size)
{
    m_game_name_position
        = { m_width / 2 - MeasureText(m_games_names[m_game_index].c_str(), font_size) / 2 };

    DrawText(m_games_names[m_game_index].c_str(), m_game_name_position, 600, font_size, RAYWHITE);
}
void Menu::draw_game_image(float dt)
{
    int texture_width = m_games_images[m_game_index].width;
    int texture_height = m_games_images[m_game_index].height;

    int posX { m_width / 2 - texture_width / 2 };
    int posX2 { m_width + texture_width };
    int dir = 1;
    float tween_value = static_cast<float>(m_width) / 2;

    if (m_animation_state != AnimationState::NONE) {
        int animation_offset_x = static_cast<int>(tween(tween_value, 1.0F - m_animation_timer));

        if (m_animation_state == AnimationState::PLAYING_LEFT)
            dir *= -1;

        int x = m_width / 2 + animation_offset_x * dir;
        posX = x - texture_width / 2;
        posX2 = posX - m_width / 2 * dir - texture_width / 2 * dir;

        m_animation_timer += dt;
        if (m_animation_timer >= 1.0F) {
            m_animation_state = AnimationState::NONE;
            m_animation_timer = 0.0F;
        }
    }

    int posY = m_height / 2 - texture_height / 2;

    DrawTexture(m_games_images[m_last_game_index], posX2, posY, RAYWHITE);
    DrawTexture(m_games_images[m_game_index], posX, posY, RAYWHITE);
}

float tween(float value, float x) { return value * (x * x * x); }

RenderTexture2D Menu::framebuffer() { return m_framebuffer; }

}
