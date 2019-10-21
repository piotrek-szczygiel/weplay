#pragma once
#include "Matrix.hpp"
#include "Shape.hpp"

namespace Tetris {

class Piece {
private:
    const Shape& shape_;
    int x_;
    int y_;
    int rotation_;

public:
    Piece(const Shape& shape)
        : shape_ { shape }
        , rotation_ { 0 }
    {
        x_ = static_cast<int>(
            static_cast<float>(WIDTH) / 2.0 - static_cast<float>(shape_.grids[rotation_].width) / 2.0F);
        y_ = VANISH - shape_.grids[rotation_].height - shape_.grids[rotation_].offsetY;
    }

    int x()
    {
        return x_;
    }

    int y()
    {
        return y_;
    }
};

}
