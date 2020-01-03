#include "Player.hpp"
#include "../Util.hpp"

namespace Pong {

void Player::update(float dt, int min_height, int max_height)
{
    if (m_controls.up)
        m_speed.y = -1.0F;
    else if (m_controls.down)
        m_speed.y = 1.0F;
    else
        m_speed.y = 0.0F;

    m_position.y += m_speed.y * m_speed_factor * dt;

    auto min = static_cast<float>(min_height);

    m_position.y = clamp(m_position.y, min, max_height - PLAYER_HEIGHT - 10.0F);

    if (m_power_up_timer > 0.0F) {
        m_power_up_timer -= dt;

        float height_diff = PLAYER_POWER_UP_HEIGHT - PLAYER_HEIGHT;
        m_offset_y = -height_diff / 2;

        if (m_position.y - (height_diff / 2.0F) < 10.0F) { // if palette exceed top border
            m_position.y = (height_diff / 2.0F) + 10.0F;
        } else if (m_position.y + PLAYER_HEIGHT + (height_diff / 2.0F)
            > max_height - 10.0F) { // if palete exceed bottom border
            m_position.y = max_height - 10.0F - (height_diff / 2.0F) - PLAYER_HEIGHT;
        }

        m_height = PLAYER_POWER_UP_HEIGHT;
    } else {
        m_offset_y = 0.0F;
        m_height = PLAYER_HEIGHT;
    }
}

void Player::init_round(Vector2 pos)
{
    if (m_power_up_timer >= 0.0F) { // reset power up and player height
        m_power_up_timer = 0.0F;
        m_height = PLAYER_HEIGHT;
    }

    m_position = { pos.x - m_width / 2.0F, pos.y - m_height / 2.0F };
}

void Player::draw()
{
    Color color = RAYWHITE;
    if (m_power_up_timer > 0.0F) {
        // full power up color {255, 255, 0, 255}
        // after normalization:
        // r = color_norm.x
        // g = color_norm.y
        // b = color_norm.z
        // a = color_norm.w
        Vector4 color_norm = ColorNormalize({ 255, 255, 0, 255 });

        float duration = PLAYER_POWER_UP_DURATION; // time duration (maximal)
        float time = duration - m_power_up_timer;

        // compute float color value <0.0F, 1.0F>
        float b = sin_out_easing(time, color_norm.z, 1.0F - color_norm.z, duration);

        // compute char color value
        unsigned char b_val = static_cast<unsigned char>(255.0F * b);

        color = { 255, 255, b_val, 255 };
    }
    DrawRectangle(static_cast<int>(m_position.x), static_cast<int>(m_position.y + m_offset_y),
        static_cast<int>(m_width), static_cast<int>(m_height), color);
};

}
