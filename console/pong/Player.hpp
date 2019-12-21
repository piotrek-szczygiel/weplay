#pragma once

#include <raylib.h>

namespace Pong {

struct PlayerControls {
    bool up;
    bool down;
};

class Player {
public:
    int m_score;
    float m_width;
    float m_height;
    Vector2 m_position;
    Vector2 m_speed;
    PlayerControls m_controls;

    Player()
        : m_width { 17.0F }
        , m_height { 180.0F }
        , m_speed_factor { 600.0F }
        , m_controls {}
        , m_score {}
        , m_position {}
        , m_speed {}
    {
    }

    ~Player() = default;

    void update(float dt, int min_height, int max_height);
    void draw() { DrawRectangle(m_position.x, m_position.y, m_width, m_height, RAYWHITE); };
    void set_pos(Vector2 pos) { m_position = { pos.x - m_width / 2.0F, pos.y - m_height / 2.0F }; };
    Rectangle get_player_rect()
    {
        return { m_position.x, m_position.y, static_cast<float>(m_width),
            static_cast<float>(m_height) };
    };

private:
    float m_speed_factor;
};
}
