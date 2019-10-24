#include "Pong.hpp"
#include <boost/log/trivial.hpp>

namespace Pong {

Vector2 computeBallSpeed(Vector2 v);

void Pong::Pong::update(std::shared_ptr<ControllerState> state)
{
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_W) || state->forward.load()) {
        m_player_1.speed.y = -1.0F;
    } else if (IsKeyDown(KEY_S)) {
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
        if (m_ball.position.x >= m_player_1.position.x) {
            m_ball.speed = computeBallSpeed(
                { -m_ball.speed.x, m_ball.speed.y + m_player_1.speed.y * FRICTION });
            m_ball.position.x = m_player_1.position.x + PLAYER_WIDTH + BALL_RADIUS + 1;
        }
    }

    if (CheckCollisionCircleRec(center, BALL_RADIUS, rect_2)) {
        if (m_ball.position.x <= m_player_2.position.x - PLAYER_WIDTH) {
            m_ball.speed = computeBallSpeed(
                { -m_ball.speed.x, m_ball.speed.y + m_player_2.speed.y * FRICTION });
            m_ball.position.x = m_player_2.position.x - BALL_RADIUS - 1;
        }
    }

    m_ball.position.x += m_ball.speed.x * dt * BALL_SPEED;
    m_ball.position.y += m_ball.speed.y * dt * BALL_SPEED;

    if (m_ball.position.x < 0) {
        m_player_2.score += 1;
    } else if (m_ball.position.x > m_width) {
        m_player_1.score += 1;
    }
}

void Pong::Pong::draw()
{
    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);

    DrawRectangle(
        m_player_1.position.x, m_player_1.position.y, PLAYER_WIDTH, PLAYER_HEIGHT, RAYWHITE);

    DrawRectangle(
        m_player_2.position.x, m_player_2.position.y, PLAYER_WIDTH, PLAYER_HEIGHT, RAYWHITE);

    DrawCircle(m_ball.position.x, m_ball.position.y, BALL_RADIUS, RAYWHITE);

    EndTextureMode();
}

RenderTexture2D Pong::Pong::framebuffer() { return m_framebuffer; }

Vector2 computeBallSpeed(Vector2 v)
{
    float vXDir = copysign(1.0F, v.x);
    float vYDir = copysign(1.0F, v.y);
    float length = sqrt(v.x * v.x + v.y * v.y);
    v = { v.x / length, v.y / length };
    if (abs(v.y) >= 0.90F) {
        v.y = 0.90F * vYDir;
        v.x = sqrt(1 - v.y * v.y) * vXDir;
    }
    return v;
}
}