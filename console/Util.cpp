#include "Util.hpp"

thread_local std::mt19937 gen { std::random_device {}() }; // NOLINT

bool Between(float x, float y, float threshold) { return std::fabs(x - y) < threshold; }

float Fade(float x, float y, float offset)
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