#include "Piece.hpp"
#include "Matrix.hpp"
#include <raylib.h>

namespace Tetris {
void Piece::draw(int drawX, int drawY) const
{
    int x = drawX + x_ * BLOCK_SIZE;
    int y = drawY + (y_ - VANISH) * BLOCK_SIZE;

    shape_.draw(x, y, rotation_);
}
}
