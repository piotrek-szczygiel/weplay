#include "PowerUp.hpp"

namespace Pong {

void PowerUp::check_collision(Ball* ball)
{
    Rectangle rect_1 { m_position.x, m_position.y, m_width, m_height };

    m_colliding = CheckCollisionCircleRec(ball->m_position, ball->m_radius, rect_1);
}

}
