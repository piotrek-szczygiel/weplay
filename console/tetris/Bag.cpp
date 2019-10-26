#include <algorithm>
#include "Bag.hpp"

namespace Tetris {

ShapeType Bag::pop()
{
    ShapeType shape = m_bag.front();
    m_bag.pop_front();
    fill_all();
    return shape;
}

ShapeType Bag::peek() { return m_bag.front(); }

void Bag::fill_all()
{
    if (m_bag.empty()) {
        fill_7();
        fill_7();
    } else if (m_bag.size() == 7) {
        fill_7();
    }
}

void Bag::fill_7()
{
    std::vector<ShapeType> temp;
    std::copy(SHAPES_ALL.begin(), SHAPES_ALL.end(), std::back_inserter(temp));
    std::shuffle(temp.begin(), temp.end(), Context::instance().rng);
    std::copy(temp.begin(), temp.end(), std::back_inserter(m_bag));
}

}
