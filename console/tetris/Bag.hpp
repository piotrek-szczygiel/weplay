#pragma once
#include "Shape.hpp"
#include <deque>
#include <random>
#include <vector>

namespace Tetris {

class Bag {
public:
    Bag() { fill_all(); }

    ShapeType pop();
    ShapeType peek();

private:
    std::deque<ShapeType> m_bag {};
    std::mt19937 m_generator { std::random_device {}() };

    void fill_all();
    void fill_7();
};

}
