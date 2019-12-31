#pragma once

#include <raylib.h>

namespace Pong {

constexpr float PLAYER_HEIGHT { 180.0F };
constexpr float PLAYER_POWER_UP_HEIGHT { 300.0F };
constexpr float PLAYER_POWER_UP_DURATION { 10.0F };

struct PlayerControls {
    bool up;
    bool down;
};

class Player {
public:
    int m_score;
    float m_width;
    float m_height;
    float m_power_up_timer;
    Vector2 m_position;
    Vector2 m_speed;
    PlayerControls m_controls;

    Player()
        : m_width { 17.0F }
        , m_height { PLAYER_HEIGHT }
        , m_speed_factor { 600.0F }
        , m_controls {}
        , m_score {}
        , m_position {}
        , m_speed {}
        , m_power_up_timer { 0.0F }
    {
    }

    ~Player() = default;

    void update(float dt, int min_height, int max_height);
    void init_round(Vector2 pos);
    void draw();

    Rectangle get_player_rect()
    {
        return { m_position.x, m_position.y, static_cast<float>(m_width),
            static_cast<float>(m_height) };
    };

private:
    float m_speed_factor;
};
}
