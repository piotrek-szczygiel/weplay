#pragma once
#include "Matrix.hpp"
#include "Shape.hpp"
#include <functional>

namespace Tetris {

class Piece {
public:
    using CollisionFun = std::function<bool(const Piece&)>;

    Piece(const Shape& shape)
        : shape_ { &const_cast<Shape&>(shape) }
        , rotation_ { 0 }
    {
        x_ = static_cast<int>(
            static_cast<float>(WIDTH) / 2.0F - static_cast<float>(shape_->grids[rotation_].width) / 2.0F);
        y_ = VANISH - shape_->grids[rotation_].height - shape_->grids[rotation_].offsetY;
    }

    int x() const
    {
        return x_;
    }

    int y() const
    {
        return y_;
    }

    const ShapeGrid& grid() const
    {
        return shape_->grids[rotation_];
    }

    bool move(int x, int y, CollisionFun collision);
    void draw(int drawX, int drawY) const;

private:
    Shape* shape_;
    int x_;
    int y_;
    int rotation_;

    bool collision_(int x, int y, CollisionFun collision);
};

}