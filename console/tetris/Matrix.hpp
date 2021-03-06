#pragma once
#include "Shape.hpp"
#include <vector>

constexpr int HEIGHT { 20 };
constexpr int VANISH { 20 };
constexpr int WIDTH { 10 };

namespace Tetris {

class Piece;

using MatrixGrid = std::array<std::array<ShapeType, WIDTH>, HEIGHT + VANISH>;

class Matrix {
public:
    bool lock(const Piece& piece);
    bool collision(const Piece& piece);
    void clear_rows(const std::vector<int>& rows);
    std::vector<int> get_full_rows();
    void add_garbage_line(int hole);
    void add_garbage_lines(int count);

    void draw(int level, int draw_x, int draw_y) const;

private:
    MatrixGrid m_grid {};
};

}
