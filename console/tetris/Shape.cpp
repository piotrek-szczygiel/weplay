#include "Shape.hpp"
#include "Matrix.hpp"
#include <raylib.h>

namespace Tetris {
void Shape::draw(int drawX, int drawY, int rotation) const
{
    const auto& grid { grids[rotation] };

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            ShapeType s { grid.grid[y][x] };
            if (s == 0) {
                continue;
            }

            DrawRectangle(drawX + x * BLOCK_SIZE, drawY + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SHAPE_COLORS[s]);
        }
    }
}
}
