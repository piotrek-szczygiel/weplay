#include "Pong.hpp"
#include "../Util.hpp"
#include <boost/format.hpp>

namespace Pong {

Vector2 computeBallSpeed(Vector2 v);

void Pong::update(std::shared_ptr<ControllerState> state)
{
    if (IsKeyPressed(KEY_Q) || state->buttons[8]) {
        m_state_change = StateChange::Menu;
        return;
    }

    float dt = GetFrameTime();

    if (m_state == PLAYING) {
        if (IsKeyDown(KEY_W) || state->buttons[0]) {
            m_player_1.speed.y = -1.0F;
        } else if (IsKeyDown(KEY_S) || state->buttons[1]) {
            m_player_1.speed.y = 1.0F;
        } else {
            m_player_1.speed.y = 0.0F;
        }

        if (IsKeyDown(KEY_UP) || state->buttons[4]) {
            m_player_2.speed.y = -1.0F;
        } else if (IsKeyDown(KEY_DOWN) || state->buttons[5]) {
            m_player_2.speed.y = 1.0F;
        } else {
            m_player_2.speed.y = 0.0F;
        }

        m_player_1.position.y += m_player_1.speed.y * dt * PLAYER_SPEED;
        m_player_2.position.y += m_player_2.speed.y * dt * PLAYER_SPEED;

        m_player_1.position.y
            = clamp(m_player_1.position.y, 10.0F, m_height - PLAYER_HEIGHT - 10.0F);
        m_player_2.position.y
            = clamp(m_player_2.position.y, 10.0F, m_height - PLAYER_HEIGHT - 10.0F);

        if (m_ball.position.y < BALL_RADIUS) {
            m_ball.speed.y = -m_ball.speed.y;
            m_ball.position.y = BALL_RADIUS;
        } else if (m_ball.position.y > m_height - BALL_RADIUS) {
            m_ball.speed.y = -m_ball.speed.y;
            m_ball.position.y = m_height - BALL_RADIUS;
        }

        Vector2 center { m_ball.position.x, m_ball.position.y };

        RlRectangle rect_1 {
            m_player_1.position.x,
            m_player_1.position.y,
            PLAYER_WIDTH,
            PLAYER_HEIGHT,
        };
        RlRectangle rect_2 {
            m_player_2.position.x,
            m_player_2.position.y,
            PLAYER_WIDTH,
            PLAYER_HEIGHT,
        };

        if (CheckCollisionCircleRec(center, BALL_RADIUS, rect_1)) {
            m_ball.speed = computeBallSpeed(
                { -m_ball.speed.x, m_ball.speed.y + m_player_1.speed.y * FRICTION });
            m_ball.position.x = m_player_1.position.x + PLAYER_WIDTH + BALL_RADIUS + 1;
        }

        if (CheckCollisionCircleRec(center, BALL_RADIUS, rect_2)) {
            m_ball.speed = computeBallSpeed(
                { -m_ball.speed.x, m_ball.speed.y + m_player_2.speed.y * FRICTION });
            m_ball.position.x = m_player_2.position.x - BALL_RADIUS - 1;
        }

        m_ball.position.x += m_ball.speed.x * dt * BALL_SPEED;
        m_ball.position.y += m_ball.speed.y * dt * BALL_SPEED;

        if (m_ball.position.x < 0) {
            add_score(1);
            restart();
        } else if (m_ball.position.x > m_width) {
            add_score(0);
            restart();
        }
    } else if (m_state == SCORING) {
        if (m_animation_timer >= 2.0F) {
            m_anim_state = SHADOWING;
            m_animation_timer = 0.0F;
            change_state(PLAYING);
        } else if (m_animation_timer >= 1.0F) {
            m_anim_state = LIGHTING;
        }
        m_animation_timer += dt;
    }
}

void Pong::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);
    if (m_state == PLAYING) {
        DrawRectangle(static_cast<int>(m_player_1.position.x),
            static_cast<int>(m_player_1.position.y), static_cast<int>(PLAYER_WIDTH),
            static_cast<int>(PLAYER_HEIGHT), RAYWHITE);

        DrawRectangle(static_cast<int>(m_player_2.position.x),
            static_cast<int>(m_player_2.position.y), static_cast<int>(PLAYER_WIDTH),
            static_cast<int>(PLAYER_HEIGHT), RAYWHITE);

        DrawCircle(static_cast<int>(m_ball.position.x), static_cast<int>(m_ball.position.y),
            BALL_RADIUS, RAYWHITE);

        RlDrawText(m_score.c_str(), m_score_position, 15, FONT_SIZE, WHITE);
    } else if (m_state == SCORING) {
        int score_position_x { 0 };
        int score_position_y { static_cast<int>(
            m_height / 2 - (static_cast<float>(ANIM_FONT_SIZE) / 2)) };

        if (m_anim_state == SHADOWING) {
            score_position_x = text_position_center(m_last_score, ANIM_FONT_SIZE);
            Color color { 255, 255, 255,
                static_cast<unsigned char>(255 - 240 * m_animation_timer) };

            RlDrawText(
                m_last_score.c_str(), score_position_x, score_position_y, ANIM_FONT_SIZE, color);
        } else if (m_anim_state == LIGHTING) {
            score_position_x = text_position_center(m_score, ANIM_FONT_SIZE);
            Color color { 255, 255, 255,
                static_cast<unsigned char>(0 + 240 * (m_animation_timer - 1.0F)) };

            RlDrawText(m_score.c_str(), score_position_x, score_position_y, ANIM_FONT_SIZE, color);
        }
    }

    EndTextureMode();
}

RenderTexture2D Pong::framebuffer() { return m_framebuffer; }

void Pong::restart()
{
    m_player_1.position = {
        20,
        m_height / 2.0F - static_cast<float>(PLAYER_HEIGHT) / 2.0F,
    };

    m_player_2.position = {
        m_width - 20 - PLAYER_WIDTH,
        m_height / 2.0F - static_cast<float>(PLAYER_HEIGHT) / 2.0F,
    };

    float modAngle = random(-PI / 8.0F, PI / 8.0F, m_gen);
    float dirX = signbit(m_ball.speed.x) ? -1.0F : 1.0F;

    m_ball = { { static_cast<float>(m_width / 2), static_cast<float>(m_height / 2) },
        { computeBallSpeed(Vector2 { cos(modAngle) * dirX * -1.0F, sin(modAngle) }) } };

    m_score = str(boost::format("%d : %d") % m_player_1.score % m_player_2.score);
    m_score_position = text_position_center(m_score, FONT_SIZE);
}
void Pong::add_score(int id)
{
    if (id == 0) {
        m_player_1.score += 1;
    } else if (id == 1) {
        m_player_2.score += 1;
    }
    m_last_score = m_score;
    change_state(SCORING);
}
void Pong::change_state(Pong::State state) { m_state = state; }

int Pong::text_position_center(std::string& string, int font_size)
{
    return (static_cast<int>(m_width) - MeasureText(string.c_str(), font_size)) / 2;
}

Vector2 computeBallSpeed(Vector2 v)
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

}
