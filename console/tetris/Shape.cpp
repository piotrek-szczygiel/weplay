#include "Shape.hpp"
#include "DrawBlock.hpp"
#include <raylib.h>

namespace Tetris {

void Shape::draw(int draw_x, int draw_y, int rotation, int size, bool small, bool ghost) const
{
    const auto& grid { grids[rotation] };

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            ShapeType t { grid.grid[y][x] };
            if (t == 0) {
                continue;
            }

            // auto color = SHAPE_COLORS[s];
            // if (ghost) {
            //     color.a = 64;
            // }
            //
            // DrawRectangle(draw_x + x * size, draw_y + y * size, size, size, color);

            unsigned char alpha = ghost ? 64 : 255;
            DrawBlock(t, draw_x + x * size, draw_y + y * size, alpha, small);
        }
    }
}

}
