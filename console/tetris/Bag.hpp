#pragma once
#include "Shape.hpp"
#include <deque>
#include <vector>

namespace Tetris {

class Bag {
public:
    Bag()
    {
        fill_();
    }

    ShapeType pop();
    ShapeType peek();

private:
    std::deque<ShapeType> bag_ {};

    void fill_();
    void fill7_();
};

}
