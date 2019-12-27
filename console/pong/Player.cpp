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

    float min = static_cast<float>(min_height);

    m_position.y = clamp(m_position.y, min, max_height - m_height - 10.0F);

    if (m_power_up_timer > 0.0F) {
        m_power_up_timer -= dt;
        if (m_position.y + PLAYER_POWER_UP_HEIGHT > max_height) {
            m_position.y -= (m_position.y + PLAYER_POWER_UP_HEIGHT) - max_height;
        }
        m_height = PLAYER_POWER_UP_HEIGHT;
    } else {
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

}
