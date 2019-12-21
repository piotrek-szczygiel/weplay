#include "Pong.hpp"
#include "../Util.hpp"

namespace Pong {

void Pong::update(const std::vector<ControllerState>& controllers)
{
    const auto& s1 = controllers[0];
    const auto& s2 = controllers[1];

    if (IsKeyPressed(KEY_Q) || s1.buttons[8] || s2.buttons[8]) {
        m_state_change = StateChange::Menu;
        return;
    }

    float dt = GetFrameTime();

    if (m_game_state == State::PLAYING) {

        PlayerControls player_1_controls {
            IsKeyDown(KEY_W) || s1.buttons[0],
            IsKeyDown(KEY_S) || s1.buttons[1],
        };

        PlayerControls player_2_controls {
            IsKeyDown(KEY_UP) || s2.buttons[0],
            IsKeyDown(KEY_DOWN) || s2.buttons[1],
        };

        m_player_1.m_controls = player_1_controls;
        m_player_2.m_controls = player_2_controls;

        m_player_1.update(dt, 10, m_height);
        m_player_2.update(dt, 10, m_height);

        m_ball.check_collision(&m_player_1);
        m_ball.check_collision(&m_player_2);

        m_ball.update(dt, m_height);

        if (m_ball.m_position.x < 0) {
            add_score(1);
            new_round();
        } else if (m_ball.m_position.x > m_width) {
            add_score(0);
            new_round();
        }
    } else if (m_game_state == State::SCORING) {
        if (m_animation_timer >= 1.8F) {
            m_game_state = State::PLAYING;
            m_anim_state = AnimationState::SHADOWING;
            m_animation_timer = 0.0F;
            m_animation_timer_shift = 0.0F;
        } else if (m_animation_timer >= 1.5F) {
            m_anim_state = AnimationState::NONE;
        } else if (m_animation_timer >= 1.0F) {
            m_anim_state = AnimationState::SHADOWING;
            m_animation_timer_shift = 1.0F;
        } else if (m_animation_timer >= 0.5F) {
            m_anim_state = AnimationState::LIGHTING;
            m_animation_timer_shift = 0.5F;
        }
        m_animation_timer += dt;
    }
}

void Pong::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);
    if (m_game_state == State::PLAYING) {
        DrawRectangle(0, 0, static_cast<int>(m_width), static_cast<int>(m_height), BLACK);

        m_player_1.draw();
        m_player_2.draw();

        m_ball.draw();

        DrawText(m_score.c_str(), m_score_position, 15, m_font_size, WHITE);
    } else if (m_game_state == State::SCORING) {
        int score_position_x { text_position_center(m_score.c_str(), m_animation_font_size) };
        int score_position_y { static_cast<int>(
            m_height / 2 - (static_cast<float>(m_animation_font_size) / 2)) };

        if (m_anim_state == AnimationState::SHADOWING) {
            Color color { 255, 255, 255,
                static_cast<unsigned char>(
                    255 - tween(230.0F, 2 * (m_animation_timer - m_animation_timer_shift))) };

            DrawText(
                m_score.c_str(), score_position_x, score_position_y, m_animation_font_size, color);
        } else if (m_anim_state == AnimationState::LIGHTING) {
            Color color { 255, 255, 255,
                static_cast<unsigned char>(
                    0 + tween(230.0F, 2 * (m_animation_timer - m_animation_timer_shift))) };

            DrawText(
                m_score.c_str(), score_position_x, score_position_y, m_animation_font_size, color);
        }
    }

    EndTextureMode();
}

RenderTexture2D Pong::framebuffer() { return m_framebuffer; }

void Pong::new_round()
{
    m_player_1.set_pos({ 20.0F, static_cast<float>(m_height / 2.0F) });
    m_player_2.set_pos(
        { static_cast<float>(m_width - 20.0F), static_cast<float>(m_height / 2.0F) });

    m_ball.set_pos();

    m_score = TextFormat("%d : %d", m_player_1.m_score, m_player_2.m_score);
    m_score_position = text_position_center(m_score.c_str(), m_font_size);
}

void Pong::add_score(int id)
{
    if (id == 0) {
        m_player_1.m_score += 1;
    } else if (id == 1) {
        m_player_2.m_score += 1;
    }
    m_game_state = State::SCORING;
}

int Pong::text_position_center(const char* text, int font_size)
{
    return (static_cast<int>(m_width) - MeasureText(text, font_size)) / 2;
}

float Pong::tween(float value, float x) { return value * (x * x * x); }
}
