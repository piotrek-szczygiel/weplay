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
    if (collision(piece)) {
        return false;
    }

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

bool Matrix::collision(const Piece& piece)
{
    auto grid = piece.grid();
    int x = piece.x() + grid.offsetX;
    int y = piece.y() + grid.offsetY;

    if (x < 0 || x + grid.width > WIDTH || y < 0 || y + grid.height > HEIGHT + VANISH) {
        return true;
    }

    for (int my = 0; my < grid.height; ++my) {
        for (int mx = 0; mx < grid.width; ++mx) {
            auto s = grid.grid[my + grid.offsetY][mx + grid.offsetX];
            if (s != 0 && grid_[y + my][x + mx] != 0) {
                return true;
            }
        }
    }

    return false;
}

void Matrix::draw(int drawX, int drawY) const
{
    constexpr int outline = 2;

    RlRectangle outlineRect {
        static_cast<float>(drawX - outline),
        static_cast<float>(drawY - BLOCK_SIZE / 2 - outline),
        static_cast<float>(WIDTH * BLOCK_SIZE + outline * 2),
        static_cast<float>(HEIGHT * BLOCK_SIZE + outline * 2 + BLOCK_SIZE / 2),
    };

    DrawRectangleLinesEx(outlineRect, outline, GRAY);
    DrawRectangle(drawX, drawY - BLOCK_SIZE * 2 - BLOCK_SIZE / 2 - outline, WIDTH * BLOCK_SIZE, BLOCK_SIZE * 2, BLACK);

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
