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

    char m_player_1_score_text[10];
    sprintf(m_player_1_score_text, "%d", m_player_1.score);
    RlDrawText(m_player_1_score_text, static_cast<int>(m_width / 2 - 100), 15, FONT_SIZE, WHITE);

    char delim[2];
    sprintf(delim, "%c", ':');
    RlDrawText(delim, static_cast<int>(m_width / 2), 15, FONT_SIZE, WHITE);

    char m_player_2_score_text[10];
    sprintf(m_player_2_score_text, "%d", m_player_2.score);
    RlDrawText(m_player_2_score_text, static_cast<int>(m_width / 2 + 100), 15, FONT_SIZE, WHITE);

    EndTextureMode();
}

RenderTexture2D Pong::Pong::framebuffer() { return m_framebuffer; }

void Pong::restart()
{
    m_player_1.position.y = static_cast<float>(m_height / 2 - PLAYER_HEIGHT / 2);
    m_player_2.position.y = static_cast<float>(m_height / 2 - PLAYER_HEIGHT / 2);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-PI / 8, PI / 8);
    float modAngle = dis(gen);
    float dirX = signbit(m_ball.speed.x) ? -1.0F : 1.0F;
    m_ball = { { static_cast<float>(m_width / 2), static_cast<float>(m_height / 2) },
        { computeBallSpeed(Vector2 { cos(modAngle) * dirX * -1.0F, sin(modAngle) }) } };
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