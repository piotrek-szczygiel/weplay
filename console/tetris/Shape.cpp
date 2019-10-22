#include "Shape.hpp"
#include "Matrix.hpp"
#include <raylib.h>

namespace Tetris {
void Shape::draw(int draw_x, int draw_y, int rotation, bool ghost) const
{
    const auto& grid { grids[rotation] };

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            ShapeType s { grid.grid[y][x] };
            if (s == 0) {
                continue;
            }

            auto color = SHAPE_COLORS[s];
            if (ghost) {
                color.a = 64;
            }

            DrawRectangle(
                draw_x + x * BLOCK_SIZE, draw_y + y * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, color);
        }
    }
}
}
