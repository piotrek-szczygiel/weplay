#include "Util.hpp"

bool threshold(float x, float y, float threshold) { return std::fabs(x - y) < threshold; }

float fade(float x, float y, float offset)
{
    if (x > y) {
        x -= offset;
        if (x < y) {
            x = y;
        }
    } else if (x < y) {
        x += offset;
        if (x > y) {
            x = y;
        }
    }

    return x;
}
