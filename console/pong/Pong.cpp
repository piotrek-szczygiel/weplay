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
        if (m_power_up_timer > 0.0F) {
            m_power_up_timer -= dt;
        } else {
            m_power_up_timer = POWER_UP_DELAY;

            int random_power_up_type = m_dis_int(m_gen);

            float pos = m_dis_real(m_gen);

            if (random_power_up_type == 0) {
                m_power_ups.emplace_back(PowerUpType::PALETTE_SIZE, pos);
            } else {
                m_power_ups.emplace_back(PowerUpType::BALL_SPEED, pos);
            }
        }

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

        auto it = m_power_ups.begin();
        while (it != m_power_ups.end()) {
            it->check_collision(&m_ball);

            it->update(dt);

            if (it->m_colliding) {
                if (it->m_type == PowerUpType::PALETTE_SIZE) {
                    if (m_ball.m_speed.x > 0.0F) {
                        m_player_1.m_power_up_timer = 10.0F;
                    } else {
                        m_player_2.m_power_up_timer = 10.0F;
                    }
                } else if (it->m_type == PowerUpType::BALL_SPEED) {
                    m_ball.m_power_up_timer = 5.0F;
                }

                it->to_remove = true;
            }

            if (it->m_position.y >= m_height) {
                it->to_remove = true;
            }

            ++it;
        }

        for (int i = 0; i < m_power_ups.size(); ++i) {
            if (m_power_ups[i].to_remove) {
                m_power_ups.erase(m_power_ups.begin() + i);
                --i;
            }
        }

        if (m_ball.m_position.x < 0) {
            add_score(1);
            new_round();
        } else if (m_ball.m_position.x > m_width) {
            add_score(0);
            new_round();
        }
    } else if (m_game_state == State::SCORING) {
        m_animation_timer += dt;

        if (m_animation_timer >= 0.0F && m_animation_timer < 0.5F) {
            m_animation_state = AnimationState::SHADOWING_LAST_SCORE;
        } else if (m_animation_timer >= 0.5F && m_animation_timer < 1.0F) {
            m_animation_state = AnimationState::LIGHTING;
            m_animation_timer_shift = 0.5F;
        } else if (m_animation_timer >= 1.0F && m_animation_timer < 1.5F) {
            m_animation_state = AnimationState::SHADOWING_SCORE;
            m_animation_timer_shift = 1.0F;
        } else if (m_animation_timer >= 1.5F && m_animation_timer < 1.8F) {
            m_animation_state = AnimationState::DELAY;
            m_animation_timer_shift = 1.5F;
        } else {
            m_game_state = State::PLAYING;
            m_animation_timer = 0.0F;
            m_animation_timer_shift = 0.0F;
        }
    }
}

void Pong::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);
    if (m_game_state == State::PLAYING) {
        DrawRectangle(0, 0, static_cast<int>(m_width), static_cast<int>(m_height), BLACK);

        m_ball.draw();

        for (auto& it : m_power_ups) {
            it.draw();
        }

        m_player_1.draw();
        m_player_2.draw();

        DrawText(m_score.c_str(), m_score_position, 15, m_font_size, WHITE);
    } else if (m_game_state == State::SCORING) {
        int score_position_x { text_position_center(m_score.c_str(), m_animation_font_size) };
        int last_score_position_x { text_position_center(
            m_last_score.c_str(), m_animation_font_size) };
        int score_position_y { static_cast<int>(
            m_height / 2 - (static_cast<float>(m_animation_font_size) / 2)) };

        float tween_value = tween(230.0F, 2 * (m_animation_timer - m_animation_timer_shift));
        auto alpha = static_cast<unsigned char>(255 - tween_value);

        Color shadowing_color { 255, 255, 255, alpha };

        alpha = static_cast<unsigned char>(tween_value);
        Color lighting_color { 255, 255, 255, alpha };

        if (m_animation_state == AnimationState::SHADOWING_LAST_SCORE) {
            DrawText(m_last_score.c_str(), last_score_position_x, score_position_y,
                m_animation_font_size, shadowing_color);
        } else if (m_animation_state == AnimationState::LIGHTING) {
            DrawText(m_score.c_str(), score_position_x, score_position_y, m_animation_font_size,
                lighting_color);
        } else if (m_animation_state == AnimationState::SHADOWING_SCORE) {
            DrawText(m_score.c_str(), score_position_x, score_position_y, m_animation_font_size,
                shadowing_color);
        }
    }

    EndTextureMode();
}

RenderTexture2D Pong::framebuffer() { return m_framebuffer; }

void Pong::new_round()
{
    m_power_up_timer = POWER_UP_DELAY;
    m_power_ups.clear();

    m_player_1.init_round({ 20.0F, static_cast<float>(m_height / 2.0F) });
    m_player_2.init_round(
        { static_cast<float>(m_width - 20.0F), static_cast<float>(m_height / 2.0F) });

    m_ball.init_round();

    m_last_score = m_score;

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
