#pragma once
#include "Shape.hpp"

namespace Tetris {

constexpr int HEIGHT = 20;
constexpr int VANISH = 20;
constexpr int WIDTH = 20;

using MatrixGrid = std::array<std::array<ShapeType, WIDTH>, HEIGHT + VANISH>;

class Matrix {
private:
    MatrixGrid grid;

public:
    Matrix()
        : grid {}
    {
    }
};

}
