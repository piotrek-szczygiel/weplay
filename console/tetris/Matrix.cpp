#include "Matrix.hpp"
#include "Piece.hpp"
#include <raylib.h>

namespace Tetris {

ShapeType Matrix::get(int x, int y)
{
    return grid_[y][x];
}

bool Matrix::lock(const Piece& piece)
{
    // TODO: check collision

    auto grid = piece.grid();
    int x = piece.x() + grid.offsetX;
    int y = piece.y() + grid.offsetY;

    if (y + grid.height <= VANISH) {
        return false;
    }

    for (int my = 0; my < grid.height; ++my) {
        for (int mx = 0; mx < grid.width; ++mx) {
            ShapeType s = grid.grid[my + grid.offsetY][mx + grid.offsetX];

            if (s != 0) {
                grid_[y + my][x + mx] = s;
            }
        }
    }

    return true;
}

void Matrix::draw(int drawX, int drawY) const
{
    RlRectangle outline {
        static_cast<float>(drawX),
        static_cast<float>(drawY),
        static_cast<float>(WIDTH * BLOCK_SIZE),
        static_cast<float>(HEIGHT * BLOCK_SIZE),
    };

    DrawRectangleLinesEx(outline, 2, BLUE);

    for (int y = 0; y <= HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            auto s = grid_[VANISH + y - 1][x];

            if (s == 0) {
                continue;
            }

            DrawRectangle(
                drawX + x * BLOCK_SIZE, drawY + (y - 1) * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, SHAPE_COLORS[s]);
        }
    }
}

}
