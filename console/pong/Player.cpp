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
}

}
