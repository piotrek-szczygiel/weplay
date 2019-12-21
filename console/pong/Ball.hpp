#include "Player.hpp"
#include <random>
#include <raylib.h>

namespace Pong {

constexpr float FRICTION { 0.8F };

class Ball {
public:
    Vector2 m_position;

    Ball()
        : m_radius { 10.0F }
        , m_speed_factor { 800.0F }
        , m_speed {}
        , m_position {}
    {
    }

    ~Ball() = default;

    void update(float dt, int max_height);
    void check_collision(Player* player);
    void set_pos();
    void draw()
    {
        DrawCircle(
            static_cast<int>(m_position.x), static_cast<int>(m_position.y), m_radius, RAYWHITE);
    };

private:
    float m_radius;

    float m_speed_factor;

    Vector2 m_speed;

    std::mt19937 m_gen { std::random_device {}() };

    Vector2 compute_speed(Vector2 v);
};
}
