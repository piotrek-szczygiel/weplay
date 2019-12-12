#include "Pong.hpp"
#include "../Util.hpp"

namespace Pong {

void Pong::update()
{
    // const auto& s1 = all_states->controllers[0];
    // const auto& s2 = all_states->controllers[1];

    // if (IsKeyPressed(KEY_Q) || s1.buttons[8] || s2.buttons[8]) {
    //    m_state_change = StateChange::Menu;
    //    return;
    //}

    // if (m_game_state == PLAYING) {
    //    if (IsKeyDown(KEY_W) || s1.buttons[0]) {
    //        m_player_1.speed.y = -1.0F;
    //    } else if (IsKeyDown(KEY_S) || s1.buttons[1]) {
    //        m_player_1.speed.y = 1.0F;
    //    } else {
    //        m_player_1.speed.y = 0.0F;
    //    }

    //    if (IsKeyDown(KEY_UP) || s2.buttons[0]) {
    //        m_player_2.speed.y = -1.0F;
    //    } else if (IsKeyDown(KEY_DOWN) || s2.buttons[1]) {
    //        m_player_2.speed.y = 1.0F;
    //    } else {
    //        m_player_2.speed.y = 0.0F;
    //    }

    float dt = GetFrameTime();

    m_player_1.position.y += m_player_1.speed.y * m_player_speed_factor * dt;
    m_player_2.position.y += m_player_2.speed.y * m_player_speed_factor * dt;

    m_player_1.position.y = clamp(m_player_1.position.y, 10.0F, m_height - m_racket_height - 10.0F);
    m_player_2.position.y = clamp(m_player_2.position.y, 10.0F, m_height - m_racket_height - 10.0F);

    if (m_ball.position.y < m_ball_radius) {
        m_ball.speed.y = -m_ball.speed.y;
        m_ball.position.y = m_ball_radius;
    } else if (m_ball.position.y > m_height - m_ball_radius) {
        m_ball.speed.y = -m_ball.speed.y;
        m_ball.position.y = m_height - m_ball_radius;
    }

    Vector2 center { m_ball.position.x, m_ball.position.y };

    RlRectangle rect_1 {
        m_player_1.position.x,
        m_player_1.position.y - m_collision_shift,
        m_racket_width,
        m_racket_height + m_collision_shift,
    };
    RlRectangle rect_2 {
        m_player_2.position.x,
        m_player_2.position.y - m_collision_shift,
        m_racket_width,
        m_racket_height + m_collision_shift,
    };

    if (CheckCollisionCircleRec(center, m_ball_radius, rect_1)) {
        m_ball.speed = compute_ball_speed(
            { -m_ball.speed.x, m_ball.speed.y + m_player_1.speed.y * FRICTION });
        m_ball.position.x = m_player_1.position.x + m_racket_width + m_ball_radius + 1;
    }

    if (CheckCollisionCircleRec(center, m_ball_radius, rect_2)) {
        m_ball.speed = compute_ball_speed(
            { -m_ball.speed.x, m_ball.speed.y + m_player_2.speed.y * FRICTION });
        m_ball.position.x = m_player_2.position.x - m_ball_radius - 1;
    }

    m_ball.position.x += m_ball.speed.x * m_ball_speed_factor * dt;
    m_ball.position.y += m_ball.speed.y * m_ball_speed_factor * dt;

    if (m_ball.position.x < 0)
        add_score(1);
    else if (m_ball.position.x > m_width)
        add_score(0);

    //} else if (m_game_state == SCORING) {
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
        restart();
    }
    m_animation_timer += dt;
}

void Pong::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);
    if (m_game_state == State::PLAYING) {
        DrawRectangle(0, 0, static_cast<int>(m_width), static_cast<int>(m_height), BLACK);

        DrawRectangle(static_cast<int>(m_player_1.position.x),
            static_cast<int>(m_player_1.position.y), static_cast<int>(m_racket_width),
            static_cast<int>(m_racket_height), RAYWHITE);

        DrawRectangle(static_cast<int>(m_player_2.position.x),
            static_cast<int>(m_player_2.position.y), static_cast<int>(m_racket_width),
            static_cast<int>(m_racket_height), RAYWHITE);

        DrawCircle(static_cast<int>(m_ball.position.x), static_cast<int>(m_ball.position.y),
            m_ball_radius, RAYWHITE);

        RlDrawText(m_score, m_score_position, 15, m_font_size, WHITE);
    } else if (m_game_state == State::SCORING) {
        int score_position_x { text_position_center(m_score, m_animation_font_size) };
        int score_position_y { static_cast<int>(
            m_height / 2 - (static_cast<float>(m_animation_font_size) / 2)) };

        if (m_anim_state == AnimationState::SHADOWING) {
            Color color { 255, 255, 255,
                static_cast<unsigned char>(
                    255 - tween(230.0F, 2 * (m_animation_timer - m_animation_timer_shift))) };

            RlDrawText(m_score, score_position_x, score_position_y, m_animation_font_size, color);
        } else if (m_anim_state == AnimationState::LIGHTING) {
            Color color { 255, 255, 255,
                static_cast<unsigned char>(
                    0 + tween(230.0F, 2 * (m_animation_timer - m_animation_timer_shift))) };

            RlDrawText(m_score, score_position_x, score_position_y, m_animation_font_size, color);
        }
    }

    EndTextureMode();
}

RenderTexture2D Pong::framebuffer() { return m_framebuffer; }

void Pong::restart()
{
    m_player_1.position = {
        20,
        m_height / 2.0F - static_cast<float>(m_racket_height) / 2.0F,
    };

    m_player_2.position = {
        m_width - 20 - m_racket_width,
        m_height / 2.0F - static_cast<float>(m_racket_height) / 2.0F,
    };

    float modAngle = random(-PI / 8.0F, PI / 8.0F, m_gen);
    float dirX = signbit(m_ball.speed.x) ? -1.0F : 1.0F;

    m_ball = { { static_cast<float>(m_width / 2), static_cast<float>(m_height / 2) },
        { compute_ball_speed(Vector2 { cos(modAngle) * dirX * -1.0F, sin(modAngle) }) } };

    m_score = TextFormat("%d : %d", m_player_1.score, m_player_2.score);
    m_score_position = text_position_center(m_score, m_font_size);
}

void Pong::add_score(int id)
{
    if (id == 0) {
        m_player_1.score += 1;
    } else if (id == 1) {
        m_player_2.score += 1;
    }
    m_game_state = State::SCORING;
}

int Pong::text_position_center(const char* text, int font_size)
{
    return (static_cast<int>(m_width) - MeasureText(text, font_size)) / 2;
}

Vector2 Pong::compute_ball_speed(Vector2 v)
{
    float vXDir = signbit(v.x) ? -1.0F : 1.0F;
    float vYDir = signbit(v.y) ? -1.0F : 1.0F;
    float length = sqrt(v.x * v.x + v.y * v.y);
    v = { v.x / length, v.y / length };

    if (abs(v.y) >= 0.90F) {
        v.y = 0.90F * vYDir;
        v.x = sqrt(1 - v.y * v.y) * vXDir;
    }

    return v;
}

float Pong::tween(float value, float x) { return value * (x * x * x); }
}
