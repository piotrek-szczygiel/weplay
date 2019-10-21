#include "Piece.hpp"
#include "Matrix.hpp"
#include <raylib.h>

namespace Tetris {
bool Piece::move(int x, int y, CollisionFunction collision)
{
    if (collision_(x, y, collision)) {
        return false;
    }

    x_ += x;
    y_ += y;

    return true;
}

bool Piece::rotate(bool clockwise, CollisionFunction collision)
{
    if (shape_->type == O) {
        return false;
    }

    const auto& rotationKick = shape_->kicks[rotation_];
    int lastRotation = rotation_;
    bool rotated = false;

    const auto& kicks = clockwise ? rotationKick.clockwise : rotationKick.counterClockwise;

    if (clockwise) {
        rotation_ += 1;
    } else {
        rotation_ -= 1;
    }

    if (rotation_ == -1) {
        rotation_ = 3;
    } else if (rotation_ == 4) {
        rotation_ = 0;
    }

    if (!collision(*this)) {
        rotated = true;
    } else {
        for (const auto& kick : kicks) {
            if (move(kick.x, kick.y, collision)) {
                rotated = true;
                break;
            }
        }
    }

    if (!rotated) {
        rotation_ = lastRotation;
    }

    return rotated;
}

void Piece::draw(int drawX, int drawY) const
{
    int x = drawX + x_ * BLOCK_SIZE;
    int y = drawY + (y_ - VANISH) * BLOCK_SIZE;

    shape_->draw(x, y, rotation_);
}

bool Piece::collision_(int x, int y, CollisionFunction collision)
{
    x_ += x;
    y_ += y;

    bool colliding = collision(*this);

    x_ -= x;
    y_ -= y;

    return colliding;
}
}
