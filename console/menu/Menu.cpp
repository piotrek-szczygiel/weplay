#include "Menu.hpp"
#include <boost/format.hpp>
#include <boost/log/trivial.hpp>
#include <ctime>

using boost::str, boost::format;

namespace Menu {

float tween(float value, float x);

void Menu::update(std::shared_ptr<AllControllersState> all_states)
{
    const auto& s1 = all_states->controllers[0];
    const auto& s2 = all_states->controllers[1];

    if ((IsKeyPressed(KEY_RIGHT) || s1.buttons[3] || s2.buttons[3])
        && m_animation_state == AnimationState::NONE) {
        m_last_game_index = m_game_index;
        m_game_index = m_game_index == 0 ? (GAMES - 1) : m_game_index - 1;
        m_animation_state = AnimationState::PLAYING_RIGHT;
    } else if ((IsKeyPressed(KEY_LEFT) || s1.buttons[0] || s2.buttons[0])
        && m_animation_state == AnimationState::NONE) {
        m_last_game_index = m_game_index;
        m_game_index = (m_game_index + 1) % GAMES;
        m_animation_state = AnimationState::PLAYING_LEFT;
    } else if (IsKeyPressed(KEY_ENTER) || s1.buttons[5] || s2.buttons[5]) {
        m_state_change = m_games_states[m_game_index];
    }

    m_connected = all_states->connected_num;

    m_yaw = s1.yaw;
    m_pitch = s1.pitch;
    m_roll = s1.roll;
    m_buttons = s1.buttons;
}

void Menu::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);

    DrawTextureEx(m_background[m_background_frame], { 0.0F, 0.0F }, 0.0F, 2.0F, WHITE);
    m_background_timer += GetFrameTime();
    if (m_background_timer >= 0.07) {
        m_background_timer -= 0.07;
        m_background_frame += 1;

        m_background_frame %= m_background.size();
    }

    RlDrawText(
        str(format("Connected controllers: %d") % m_connected).c_str(), 10, 70, 16, RAYWHITE);
    RlDrawText(str(format("Yaw: %d") % m_yaw).c_str(), 10, 100, 16, RAYWHITE);
    RlDrawText(str(format("Pitch: %d") % m_pitch).c_str(), 10, 130, 16, RAYWHITE);
    RlDrawText(str(format("Roll: %d") % m_roll).c_str(), 10, 160, 16, RAYWHITE);

    draw_game_name(64);
    draw_game_image(GetFrameTime() * SLIDE_SPEED);

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

void Menu::draw_game_name(int font_size)
{
    m_game_name_position
        = { m_width / 2 - MeasureText(m_games_names[m_game_index].c_str(), font_size) / 2 };

    RlDrawText(str(format("%s") % m_games_names[m_game_index]).c_str(), m_game_name_position, 600,
        font_size, RAYWHITE);
}
void Menu::draw_game_image(float dt)
{
    int texture_width = m_games_images[m_game_index].width;
    int texture_height = m_games_images[m_game_index].height;

    int posX { m_width + texture_width };
    int posX2 { m_width + texture_width };
    float tween_value = static_cast<float>(m_width) / 2;

    if (m_animation_state == AnimationState::PLAYING_RIGHT) {
        int x
            = m_width / 2 + static_cast<int>(tween(tween_value, 1.0F - (m_animation_timer - 0.0F)));
        posX = x - texture_width / 2;

        int x2 = static_cast<int>(tween(tween_value, 1.0F - (m_animation_timer - 0.0F)));
        posX2 = x2 - texture_width;

        m_animation_timer += dt;
        if (m_animation_timer >= 1.0F) {
            m_animation_state = AnimationState::ENDING;
        }
    } else if (m_animation_state == AnimationState::PLAYING_LEFT) {
        int x
            = m_width / 2 - static_cast<int>(tween(tween_value, 1.0F - (m_animation_timer - 0.0F)));
        posX = x - texture_width / 2;

        posX2 = m_width - static_cast<int>(tween(tween_value, 1.0F - (m_animation_timer - 0.0F)));

        m_animation_timer += dt;
        if (m_animation_timer >= 1.0F) {
            m_animation_state = AnimationState::ENDING;
        }
    } else if (m_animation_state == AnimationState::ENDING) {
        posX = m_width / 2 - texture_width / 2;
        m_animation_state = AnimationState::NONE;
        m_animation_timer = 0.0F;
    } else if (m_animation_state == AnimationState::NONE) {
        posX = m_width / 2 - texture_width / 2;
    }
    DrawTexture(
        m_games_images[m_last_game_index], posX2, m_height / 2 - texture_height / 2, RAYWHITE);
    DrawTexture(m_games_images[m_game_index], posX, m_height / 2 - texture_height / 2, RAYWHITE);
}

float tween(float value, float x) { return value * (x * x * x); }

RenderTexture2D Menu::framebuffer() { return m_framebuffer; }

}
