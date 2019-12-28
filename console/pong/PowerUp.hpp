#include "Ball.hpp"
#include "Context.hpp"
#include <raylib.h>

namespace Pong {

enum class PowerUpType { PALETTE_SIZE, BALL_SPEED };

class PowerUp {
public:
    bool m_colliding;
    bool to_remove;
    Vector2 m_position;

    PowerUpType m_type;

    PowerUp(PowerUpType type, float start_pos_x)
        : m_position { start_pos_x, -40.0F }
        , m_speed { 0.0F, 1.0F }
        , m_colliding { false }
        , m_speed_factor { 150.0F }
        , to_remove { false }
        , m_width { 40.0F }
        , m_height { 40.0F }
    {
        m_type = type;

        m_texture = Context::instance().power_up_textures;

        switch (m_type) {
        case PowerUpType::PALETTE_SIZE:
            m_texture_rect = { 0.0F, 0.0F, m_width, m_height };
            break;
        case PowerUpType::BALL_SPEED:
            m_texture_rect = { 40.0F, 40.0F, m_width, m_height };
            break;
        default:
            m_texture_rect = {};
            break;
        }
    }

    ~PowerUp() { }

    void update(float dt) { m_position.y += dt * m_speed.y * m_speed_factor; };
    void draw() { DrawTextureRec(m_texture, m_texture_rect, m_position, RAYWHITE); };
    void check_collision(Ball* ball);

private:
    float m_width;
    float m_height;
    Vector2 m_speed;

    float m_speed_factor;

    Texture2D m_texture;
    Rectangle m_texture_rect;
};

}
