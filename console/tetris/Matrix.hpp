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
    ShapeType get(int x, int y);

    bool lock(const Piece& piece);

    bool collision(const Piece& piece);

    void draw(int drawX, int drawY) const;

private:
    MatrixGrid grid_ {};
};

}
