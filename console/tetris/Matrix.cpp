#include "Matrix.hpp"
#include "Context.hpp"
#include "DrawBlock.hpp"
#include "Piece.hpp"
#include <random>
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

void Matrix::add_garbage_lines(int rows)
{
    static auto hole_rand
        = std::bind(std::uniform_int_distribution<int>(0, WIDTH - 1), Context::instance().rng());

    for (int row = 0; row < rows; ++row) {
        int hole = hole_rand();
        for (int y = 0; y < HEIGHT + VANISH - 1; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                m_grid[y][x] = m_grid[y + 1][x];
            }
        }

        for (int x = 0; x < WIDTH; ++x) {
            if (x == hole) {
                m_grid[HEIGHT + VANISH - 1][x] = 0;
            } else {
                m_grid[HEIGHT + VANISH - 1][x] = GARBAGE;
            }
        }
    }
}

void Matrix::draw(int level, int draw_x, int draw_y) const
{
    for (int y = 0; y <= HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            auto t = m_grid[VANISH + y - 1][x];

            DrawBlock(t, level, draw_x + x * TILE_SIZE, draw_y + (y - 1) * TILE_SIZE, 220, false);
        }
    }
}

void Matrix::draw_outline(int draw_x, int draw_y) const
{
    constexpr int outline = 2;

    RlRectangle outlineRect {
        static_cast<float>(draw_x - outline),
        static_cast<float>(draw_y - outline) - static_cast<float>(TILE_SIZE) / 2.0F,
        static_cast<float>(WIDTH * TILE_SIZE + outline * 2),
        static_cast<float>(HEIGHT * TILE_SIZE + outline * 2) + static_cast<float>(TILE_SIZE) / 2.0F,
    };

    DrawRectangleLinesEx(outlineRect, outline, GRAY);
    DrawRectangle(draw_x, draw_y - TILE_SIZE * 2 - TILE_SIZE / 2 - outline, WIDTH * TILE_SIZE,
        TILE_SIZE * 2, BLACK);
}

}
