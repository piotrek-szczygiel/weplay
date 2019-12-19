#include "Menu.hpp"
#include <ctime>

namespace Menu {

int sin_out_easing(float time, int start_pos, int distance, float duration);

void Menu::update(const std::vector<ControllerState>& controllers)
{
    const auto& s1 = controllers[0];
    const auto& s2 = controllers[1];

    bool input_right = IsKeyPressed(KEY_RIGHT) || s1.buttons[3] || s2.buttons[3];
    bool input_left = IsKeyPressed(KEY_LEFT) || s1.buttons[0] || s2.buttons[0];
    bool input_enter = IsKeyPressed(KEY_ENTER) || s1.buttons[5] || s2.buttons[5];

    if (input_right) {
        if (m_animation_state == AnimationState::NONE) {
            m_last_game_index = m_game_index;
            m_game_index = (m_game_index + 1) % GAMES;
            m_animation_state = AnimationState::START_RIGHT;
        }
    }
    else if (input_left) {
        if (m_animation_state == AnimationState::NONE) {
            m_last_game_index = m_game_index;
            m_game_index = m_game_index == 0 ? (GAMES - 1) : m_game_index - 1;
            m_animation_state = AnimationState::START_LEFT;
        }
    }

    if (input_enter) {
        m_state_change = m_logos[m_game_index].game_state;
    }

    m_connected
        = std::count_if(controllers.begin(), controllers.end(), [](auto s) { return s.connected; });

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
    draw_game_image(GetFrameTime());

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
    int text_lenght = MeasureText(m_logos[m_game_index].name.c_str(), font_size) / 2;
    m_game_name_position = { m_width / 2 - text_lenght };

    DrawText(m_logos[m_game_index].name.c_str(), m_game_name_position, 600, font_size, RAYWHITE);
}
void Menu::draw_game_image(float dt)
{
    int left_border = 0 - m_texture_width;
    int middle = m_width / 2 - m_texture_width / 2;
    int right_border = m_width;

    // SET UP ANIMATION START AND DESTINATION POSITION
    if (m_animation_state == AnimationState::START_RIGHT) {
        m_logos[m_game_index].visible = true;

        m_logos[m_last_game_index].start_pos = middle;
        m_logos[m_last_game_index].dest_pos = left_border;

        m_logos[m_game_index].start_pos = right_border;
        m_logos[m_game_index].dest_pos = middle;

        m_animation_state = AnimationState::PLAYING_RIGHT;
    }
    else if (m_animation_state == AnimationState::START_LEFT) {
        m_logos[m_game_index].visible = true;

        m_logos[m_last_game_index].start_pos = middle;
        m_logos[m_last_game_index].dest_pos = right_border;

        m_logos[m_game_index].start_pos = left_border;
        m_logos[m_game_index].dest_pos = middle;

        m_animation_state = AnimationState::PLAYING_LEFT;
    }

    // INTERPOLATE POSITIONS
    if (m_animation_state == AnimationState::PLAYING_RIGHT
        || m_animation_state == AnimationState::PLAYING_LEFT) {

        int start_pos = m_logos[m_game_index].start_pos;
        int distance = m_logos[m_game_index].dest_pos - start_pos;

        int pos_x = sin_out_easing(m_animation_timer, start_pos, distance, ANIMATION_TIME);

        m_logos[m_game_index].draw_pos_x = pos_x;

        start_pos = m_logos[m_last_game_index].start_pos;
        distance = m_logos[m_last_game_index].dest_pos - start_pos;

        pos_x = sin_out_easing(m_animation_timer, start_pos, distance, ANIMATION_TIME);

        m_logos[m_last_game_index].draw_pos_x = pos_x;

        m_animation_timer += dt;
        if (m_animation_timer > ANIMATION_TIME) {
            m_animation_state = AnimationState::NONE;
            m_animation_timer = 0.0F;
            m_logos[m_last_game_index].visible = false;
        }
    }

    for (int i = 0; i < GAMES; ++i) {
        if (m_logos[i].visible)
            DrawTexture(m_logos[i].texture, m_logos[i].draw_pos_x, m_logos[i].draw_pos_y, RAYWHITE);
    }
}

int sin_out_easing(float time, int start_pos, int distance, float duration)
{
    return distance * sin(time / duration * (PI / 2)) + start_pos;
}

RenderTexture2D Menu::framebuffer() { return m_framebuffer; }

}
