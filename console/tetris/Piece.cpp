#include "Piece.hpp"
#include "Matrix.hpp"
#include <raylib.h>

namespace Tetris {
bool Piece::move(int x, int y, CollisionFun collision)
{
    if (collision_(x, y, collision)) {
        return false;
    }

    x_ += x;
    y_ += y;

    return true;
}

void Piece::draw(int drawX, int drawY) const
{
    int x = drawX + x_ * BLOCK_SIZE;
    int y = drawY + (y_ - VANISH) * BLOCK_SIZE;

    shape_->draw(x, y, rotation_);
}

bool Piece::collision_(int x, int y, CollisionFun collision)
{
    x_ += x;
    y_ += y;

    bool colliding = collision(*this);

    x_ -= x;
    y_ -= y;

    return colliding;
}
}
