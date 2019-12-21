#include "Ball.hpp"
#include "../Util.hpp"
#include <cmath>

namespace Pong {

void Ball::check_collision(Player* player)
{

    Rectangle rect_1 { player->get_player_rect() };
    Rectangle rect_2 { m_position.x - m_radius, m_position.y - m_radius, m_radius * 2.0F,
        m_radius * 2.0F };

    if (CheckCollisionCircleRec(m_position, m_radius, rect_1)) {
        m_speed = compute_speed({ -m_speed.x, m_speed.y + player->m_speed.y * FRICTION });
        if (m_position.x > 512.0F) { // half of screen width
            m_position.x = rect_1.x - m_radius - 1.0F;
        } else {
            m_position.x = rect_1.x + rect_1.width + m_radius + 1.0F;
        }
    }
}

void Ball::set_pos()
{
    float modAngle = random(-PI / 8.0F, PI / 8.0F, m_gen);
    float dirX = std::signbit(m_speed.x) ? -1.0F : 1.0F;

    if (dirX > 0.0F) {
        m_position = { 30.0F, 384.0F };
    } else {
        m_position = { 990.0F, 384.0F };
    }

    m_speed = { compute_speed(Vector2 { cosf(modAngle) * dirX * -1.0F, sinf(modAngle) }) };
}

void Ball::update(float dt, int max_height)
{
    if (m_position.y < m_radius) {
        m_speed.y = -m_speed.y;
        m_position.y = m_radius;
    } else if (m_position.y > max_height - m_radius) {
        m_speed.y = -m_speed.y;
        m_position.y = max_height - m_radius;
    }

    m_position.x += m_speed.x * m_speed_factor * dt;
    m_position.y += m_speed.y * m_speed_factor * dt;
}

Vector2 Ball::compute_speed(Vector2 v)
{
    float vXDir = std::signbit(v.x) ? -1.0F : 1.0F;
    float vYDir = std::signbit(v.y) ? -1.0F : 1.0F;
    float length = sqrtf(v.x * v.x + v.y * v.y);
    v = { v.x / length, v.y / length };

    if (fabsf(v.y) >= 0.90F) {
        v.y = 0.90F * vYDir;
        v.x = sqrtf(1 - v.y * v.y) * vXDir;
    }

    return v;
}

}
