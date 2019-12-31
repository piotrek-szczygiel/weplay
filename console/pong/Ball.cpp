#include "Ball.hpp"
#include "../Util.hpp"
#include <cmath>

namespace Pong {

void Ball::check_collision(Player* player)
{
    Rectangle rect_1 { player->get_player_rect() };

    if (CheckCollisionCircleRec(m_position, m_radius, rect_1)) {
        m_speed = compute_speed({ -m_speed.x, m_speed.y + player->m_speed.y * FRICTION });
        if (m_position.x > 512.0F) { // half of screen width
            m_position.x = rect_1.x - m_radius - 1.0F;
        } else {
            m_position.x = rect_1.x + rect_1.width + m_radius + 1.0F;
        }
    }
}

void Ball::init_round()
{
    if (m_power_up_timer > 0.0F) { // reset power up and ball speed
        m_power_up_timer = 0.0F;
        m_speed_factor = SPEED_FACTOR;
    }

    float modAngle = m_dis(m_gen);
    float dirX = std::signbit(m_speed.x) ? -1.0F : 1.0F;

    if (dirX > 0.0F) {
        m_position = { 30.0F, 384.0F };
    } else {
        m_position = { 990.0F, 384.0F };
    }

    m_speed = { compute_speed(Vector2 { cosf(modAngle) * dirX * -1.0F, sinf(modAngle) }) };

    m_particle_system.clear();
}

void Ball::update(float dt, int max_height)
{
    if (m_power_up_timer > 0.0F) {
        m_power_up_timer -= dt;

        m_speed_factor = SPEED_FACTOR_POWER_UP;
    } else {
        m_speed_factor = SPEED_FACTOR;
    }

    if (m_position.y < m_radius) {
        m_speed.y = -m_speed.y;
        m_position.y = m_radius;
    } else if (m_position.y > max_height - m_radius) {
        m_speed.y = -m_speed.y;
        m_position.y = max_height - m_radius;
    }

    m_position.x += m_speed.x * m_speed_factor * dt;
    m_position.y += m_speed.y * m_speed_factor * dt;

    if (m_particles_delay <= 0.0F) {

        Vector2 particle_start_pos;
        particle_start_pos.x = m_position.x - 2.0F * (m_speed.x * m_speed_factor * dt);
        particle_start_pos.y = m_position.y - 2.0F * (m_speed.y * m_speed_factor * dt);

        Vector2 particle_start_speed;
        particle_start_speed.x = m_speed.x * m_speed_factor * 0.3F;
        particle_start_speed.y = m_speed.y * m_speed_factor * 0.3F;

        m_power_up_particle.position = particle_start_pos;
        m_power_up_particle.speed = particle_start_speed;

        m_normal_particle.position = particle_start_pos;
        m_normal_particle.speed = particle_start_speed;

        if (m_power_up_timer > 0.0F) {
            m_particle_system.emit(m_power_up_particle);
        } else {
            m_particle_system.emit(m_normal_particle);
        }

        m_particles_delay = PARTICLES_DELAY;
    }

    m_particles_delay -= dt;

    m_particle_system.update(dt);
}

void Ball::draw()
{
    Color color = RAYWHITE;
    if (m_power_up_timer > 0.0F) {
        float t_dur = BALL_POWER_UP_DURATION; // time duration (maximal)

        // full power up color {254, 109, 41, 255}
        unsigned char r = sin_out_easing(t_dur - m_power_up_timer, 254.0F, 255.0F - 254.0F, t_dur);
        unsigned char g = sin_out_easing(t_dur - m_power_up_timer, 109.0F, 255.0F - 109.0F, t_dur);
        unsigned char b = sin_out_easing(t_dur - m_power_up_timer, 41.0F, 255.0F - 41.0F, t_dur);

        color = { r, g, b, 255 };
    }
    m_particle_system.draw();
    DrawCircle(static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_radius, color);
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
