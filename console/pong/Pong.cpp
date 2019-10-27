#include "Pong.hpp"
#include "../Util.hpp"
#include <boost/format.hpp>

namespace Pong {

Vector2 computeBallSpeed(Vector2 v);

void Pong::Pong::update(std::shared_ptr<ControllerState> state)
{
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_W) || state->pitch > 20) {
        m_player_1.speed.y = -1.0F;
    } else if (IsKeyDown(KEY_S) || state->pitch < -20) {
        m_player_1.speed.y = 1.0F;
    } else
        m_player_1.speed.y = 0.0F;

    if (IsKeyDown(KEY_UP)) {
        m_player_2.speed.y = -1.0F;
    } else if (IsKeyDown(KEY_DOWN)) {
        m_player_2.speed.y = 1.0F;
    } else
        m_player_2.speed.y = 0.0F;

    m_player_1.position.y += m_player_1.speed.y * dt * PLAYER_SPEED;
    m_player_2.position.y += m_player_2.speed.y * dt * PLAYER_SPEED;

    if (m_player_1.position.y < 10)
        m_player_1.position.y = 10;
    if (m_player_1.position.y > m_height - PLAYER_HEIGHT - 10)
        m_player_1.position.y = m_height - PLAYER_HEIGHT - 10;

    if (m_player_2.position.y < 10)
        m_player_2.position.y = 10;
    if (m_player_2.position.y > m_height - PLAYER_HEIGHT - 10)
        m_player_2.position.y = m_height - PLAYER_HEIGHT - 10;

    if (m_ball.position.y < BALL_RADIUS) {
        m_ball.speed.y = -m_ball.speed.y;
        m_ball.position.y = BALL_RADIUS;
    } else if (m_ball.position.y > m_height - BALL_RADIUS) {
        m_ball.speed.y = -m_ball.speed.y;
        m_ball.position.y = m_height - BALL_RADIUS;
    }

    Vector2 center = { m_ball.position.x, m_ball.position.y };

    RlRectangle rect_1
        = { m_player_1.position.x, m_player_1.position.y, PLAYER_WIDTH, PLAYER_HEIGHT };
    RlRectangle rect_2
        = { m_player_2.position.x, m_player_2.position.y, PLAYER_WIDTH, PLAYER_HEIGHT };

    if (CheckCollisionCircleRec(center, BALL_RADIUS, rect_1)) {
        // if (m_ball.position.x >= m_player_1.position.x) {
        m_ball.speed
            = computeBallSpeed({ -m_ball.speed.x, m_ball.speed.y + m_player_1.speed.y * FRICTION });
        m_ball.position.x = m_player_1.position.x + PLAYER_WIDTH + BALL_RADIUS + 1;
        //}
    }

    if (CheckCollisionCircleRec(center, BALL_RADIUS, rect_2)) {
        // if (m_ball.position.x <= m_player_2.position.x - PLAYER_WIDTH) {
        m_ball.speed
            = computeBallSpeed({ -m_ball.speed.x, m_ball.speed.y + m_player_2.speed.y * FRICTION });
        m_ball.position.x = m_player_2.position.x - BALL_RADIUS - 1;
        //}
    }

    m_ball.position.x += m_ball.speed.x * dt * BALL_SPEED;
    m_ball.position.y += m_ball.speed.y * dt * BALL_SPEED;

    if (m_ball.position.x < 0) {
        m_player_2.score += 1;
        restart();
    } else if (m_ball.position.x > m_width) {
        m_player_1.score += 1;
        restart();
    }
}

void Pong::Pong::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);

    DrawRectangle(static_cast<int>(m_player_1.position.x), static_cast<int>(m_player_1.position.y),
        PLAYER_WIDTH, PLAYER_HEIGHT, RAYWHITE);

    DrawRectangle(static_cast<int>(m_player_2.position.x), static_cast<int>(m_player_2.position.y),
        PLAYER_WIDTH, PLAYER_HEIGHT, RAYWHITE);

    DrawCircle(static_cast<int>(m_ball.position.x), static_cast<int>(m_ball.position.y),
        BALL_RADIUS, RAYWHITE);

    RlDrawText(m_score.c_str(), m_score_position, 15, FONT_SIZE, WHITE);

    EndTextureMode();
}

RenderTexture2D Pong::Pong::framebuffer() { return m_framebuffer; }

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

    m_score = boost::str(boost::format("%d : %d") % m_player_1.score % m_player_2.score);
    m_score_position = (static_cast<int>(m_width) - MeasureText(m_score.c_str(), FONT_SIZE)) / 2;
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