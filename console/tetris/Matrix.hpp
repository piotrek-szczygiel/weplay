#pragma once
#include "Shape.hpp"

constexpr int HEIGHT { 20 };
constexpr int VANISH { 20 };
constexpr int WIDTH { 10 };

constexpr int BLOCK_SIZE { 8 };

namespace Tetris {

class Piece;

using MatrixGrid = std::array<std::array<ShapeType, WIDTH>, HEIGHT + VANISH>;

class Matrix {
public:
    bool lock(const Piece& piece);
    bool collision(const Piece& piece);
    void draw(int draw_x, int draw_y) const;

private:
    MatrixGrid m_grid {};
};

}
