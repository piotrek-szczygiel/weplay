#include "Piece.hpp"
#include "DrawBlock.hpp"
#include "Matrix.hpp"

namespace Tetris {

bool Piece::move(int x, int y, const CollisionFunction& collision_fun)
{
    if (collision(x, y, collision_fun)) {
        return false;
    }

    m_x += x;
    m_y += y;
    m_last_movement = Movement::MOVE;

    return true;
}

bool Piece::rotate(bool right, const CollisionFunction& collision_fun)
{
    if (m_shape.type == O) {
        return false;
    }

    const auto& rotationKick = m_shape.kicks[m_rotation];
    int lastRotation = m_rotation;
    bool rotated = false;

    const auto& kicks = right ? rotationKick.right : rotationKick.left;

    if (right) {
        m_rotation += 1;
    } else {
        m_rotation -= 1;
    }

    if (m_rotation == -1) {
        m_rotation = 3;
    } else if (m_rotation == 4) {
        m_rotation = 0;
    }

    if (!collision_fun(*this)) {
        rotated = true;
    } else {
        for (const auto& kick : kicks) {
            if (move(kick.x, kick.y, collision_fun)) {
                rotated = true;
                break;
            }
        }
    }

    if (rotated) {
        m_last_movement = Movement::ROTATE;
    } else {
        m_rotation = lastRotation;
    }

    return rotated;
}

int Piece::fall(const CollisionFunction& collision_fun)
{
    int rows = 0;
    while (move(0, 1, collision_fun)) {
        rows += 1;
    }

    if (rows > 0) {
        m_last_movement = Movement::MOVE;
    }

    return rows;
}

void Piece::draw(int draw_x, int draw_y, bool small, bool ghost) const
{
    int x = draw_x + m_x * TILE_SIZE;
    int y = draw_y + (m_y - VANISH) * TILE_SIZE;

    m_shape.draw(x, y, m_rotation, small, ghost);
}

bool Piece::collision(int x, int y, const CollisionFunction& collision_fun)
{
    m_x += x;
    m_y += y;

    bool colliding = collision_fun(*this);

    m_x -= x;
    m_y -= y;

    return colliding;
}

bool Piece::touching_floor(const CollisionFunction& collision_fun)
{
    return collision(0, 1, collision_fun);
}

}
