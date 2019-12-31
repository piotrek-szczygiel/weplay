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

unsigned char sin_out_easing(float time, float start, float distance, float duration)
{
    return static_cast<unsigned char>(distance * std::sin(time / duration * (M_PI / 2)) + start);
}

int sin_out_easing(float time, int start_pos, int distance, float duration)
{
    return static_cast<int>(distance * std::sin(time / duration * (M_PI / 2)) + start_pos);
}
