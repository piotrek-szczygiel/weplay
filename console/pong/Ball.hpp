#pragma once

#include "Player.hpp"
#include <random>
#include <raylib.h>

namespace Pong {

constexpr float FRICTION { 0.8F };
constexpr float SPEED_FACTOR { 800.0F };
constexpr float SPEED_FACTOR_POWER_UP { 1050.0F };

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
    {
    }

    ~Ball() = default;

    void update(float dt, int max_height);
    void check_collision(Player* player);
    void init_round();
    void draw()
    {
        DrawCircle(
            static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_radius, RAYWHITE);
    };


private:
    float m_speed_factor;

    std::mt19937 m_gen { std::random_device {}() };
    std::uniform_real_distribution<> m_dis { -PI / 8.0F, PI / 8.0F };

    Vector2 compute_speed(Vector2 v);
};

}
