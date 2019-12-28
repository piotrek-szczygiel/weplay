#pragma once

#include "ParticleSystem.hpp"
#include "Player.hpp"
#include <random>
#include <raylib.h>

namespace Pong {

constexpr float FRICTION { 0.8F };
constexpr float SPEED_FACTOR { 800.0F };
constexpr float SPEED_FACTOR_POWER_UP { 1050.0F };
constexpr float PARTICLES_DELAY { 0.035F };

class Ball {
public:
    Vector2 m_position;
    Vector2 m_speed;
    float m_radius;
    float m_power_up_timer;

    Ball()
        : m_radius { 10.0F }
        , m_speed_factor { SPEED_FACTOR }
        , m_speed {}
        , m_position {}
        , m_power_up_timer { 0.0F }
        , m_particles_delay { PARTICLES_DELAY }
    {
        m_normal_particle.position = { 0.0F, 0.0F };
        m_normal_particle.speed = { -2.0F, 0.0F };
        m_normal_particle.speed_variation = { 0.1F * SPEED_FACTOR, 0.05F * SPEED_FACTOR };
        m_normal_particle.size_begin = 10.0F, m_normal_particle.size_end = 0.0F,
        m_normal_particle.size_variation = 4.0F;
        m_normal_particle.color_begin = { 255, 255, 255, 255 };
        m_normal_particle.color_end = { 125, 125, 125, 0 };
        m_normal_particle.life = 0.4F;

        m_power_up_particle.position = { 0.0F, 0.0F };
        m_power_up_particle.speed = { -2.0F, 0.0F };

        m_power_up_particle.speed_variation.x = 0.15F * SPEED_FACTOR_POWER_UP;
        m_power_up_particle.speed_variation.y = 0.1F * SPEED_FACTOR_POWER_UP;

        m_power_up_particle.size_begin = 15.0F, m_power_up_particle.size_end = 0.0F,
        m_power_up_particle.size_variation = 6.0F;
        m_power_up_particle.color_begin = { 254, 109, 41, 255 };
        m_power_up_particle.color_end = { 254, 212, 123, 0 };
        m_power_up_particle.life = 0.45F;
    }

    ~Ball() = default;

    void update(float dt, int max_height);
    void check_collision(Player* player);
    void init_round();
    void draw()
    {
        m_particle_system.draw();
        DrawCircle(
            static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_radius, RAYWHITE);
    };

private:
    float m_speed_factor;
    float m_particles_delay;

    std::mt19937 m_gen { std::random_device {}() };
    std::uniform_real_distribution<> m_dis { -PI / 8.0F, PI / 8.0F };

    ParticleProps m_normal_particle, m_power_up_particle;
    ParticleSystem m_particle_system;

    Vector2 compute_speed(Vector2 v);
};

}
