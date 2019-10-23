#include "Matrix.hpp"
#include "DrawBlock.hpp"
#include "Piece.hpp"
#include <raylib.h>

namespace Tetris {

bool Matrix::lock(const Piece& piece)
{
    if (collision(piece)) {
        return false;
    }

    auto grid = piece.grid();
    int x = piece.x() + grid.x;
    int y = piece.y() + grid.y;

    if (y + grid.height <= VANISH) {
        return false;
    }

    for (int my = 0; my < grid.height; ++my) {
        for (int mx = 0; mx < grid.width; ++mx) {
            ShapeType s = grid.grid[my + grid.y][mx + grid.x];

            if (s != 0) {
                m_grid[y + my][x + mx] = s;
            }
        }
    }

    return true;
}

bool Matrix::collision(const Piece& piece)
{
    auto grid = piece.grid();
    int x = piece.x() + grid.x;
    int y = piece.y() + grid.y;

    if (x < 0 || x + grid.width > WIDTH || y < 0 || y + grid.height > HEIGHT + VANISH) {
        return true;
    }

    for (int my = 0; my < grid.height; ++my) {
        for (int mx = 0; mx < grid.width; ++mx) {
            auto s = grid.grid[my + grid.y][mx + grid.x];
            if (s != 0 && m_grid[y + my][x + mx] != 0) {
                return true;
            }
        }
    }

    return false;
}

void Matrix::draw(int draw_x, int draw_y) const
{
    for (int y = 0; y <= HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            auto t = m_grid[VANISH + y - 1][x];

            if (t == 0) {
                continue;
            }

            // DrawRectangle(draw_x + x * BLOCK_SIZE, draw_y + (y - 1) * BLOCK_SIZE, BLOCK_SIZE,
            //     BLOCK_SIZE, SHAPE_COLORS[t]);

            DrawBlock(t, draw_x + x * BLOCK_SIZE, draw_y + (y - 1) * BLOCK_SIZE, 220, false);
        }
    }
}

void Matrix::draw_outline(int draw_x, int draw_y) const
{
    constexpr int outline = 2;

    RlRectangle outlineRect {
        static_cast<float>(draw_x - outline),
        static_cast<float>(draw_y - BLOCK_SIZE / 2 - outline),
        static_cast<float>(WIDTH * BLOCK_SIZE + outline * 2),
        static_cast<float>(HEIGHT * BLOCK_SIZE + outline * 2 + BLOCK_SIZE / 2),
    };

    DrawRectangleLinesEx(outlineRect, outline, GRAY);
    DrawRectangle(draw_x, draw_y - BLOCK_SIZE * 2 - BLOCK_SIZE / 2 - outline, WIDTH * BLOCK_SIZE,
        BLOCK_SIZE * 2, BLACK);
}

std::vector<int> Matrix::get_full_rows()
{
    std::vector<int> rows {};

    for (int y = 0; y < HEIGHT + VANISH; ++y) {
        bool full { true };

        for (int x = 0; x < WIDTH; ++x) {
            if (m_grid[y][x] == 0) {
                full = false;
                break;
            }
        }

        if (full) {
            rows.push_back(y);
        }
    }

    return rows;
}

void Matrix::clear_rows(const std::vector<int>& rows)
{
    for (int row : rows) {
        for (int y = row; y > 0; --y) {
            for (int x = 0; x < WIDTH; ++x) {
                m_grid[y][x] = m_grid[y - 1][x];
            }
        }
    }
}

}
