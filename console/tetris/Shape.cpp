#include "Shape.hpp"
#include "DrawBlock.hpp"
#include <raylib.h>

namespace Tetris {

void Shape::draw(int level, int draw_x, int draw_y, int rotation, bool small, bool ghost) const
{
    const auto& grid { grids[rotation] };

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            ShapeType t { grid.grid[y][x] };
            if (t == 0) {
                continue;
            }

            unsigned char alpha = ghost ? 64 : 255;
            int size = small ? TILE_SIZE_SMALL : TILE_SIZE;
            DrawBlock(t, level, draw_x + x * size, draw_y + y * size, alpha, small);
        }
    }
}

}
