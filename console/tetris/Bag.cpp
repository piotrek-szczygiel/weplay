#include "Bag.hpp"

namespace Tetris {
ShapeType Bag::pop()
{
    ShapeType shape = bag_.front();
    bag_.pop_front();
    fill_();
    return shape;
}

ShapeType Bag::peek()
{
    return bag_.front();
}

void Bag::fill_()
{
    if (bag_.size() == 0) {
        fill7_();
        fill7_();
    } else if (bag_.size() == 7) {
        fill7_();
    }
}

void Bag::fill7_()
{
    std::vector<ShapeType> temp;
    std::copy(SHAPES_ALL.begin(), SHAPES_ALL.end(), std::back_inserter(temp));
    std::random_shuffle(temp.begin(), temp.end());
    std::copy(temp.begin(), temp.end(), std::back_inserter(bag_));
}
}
