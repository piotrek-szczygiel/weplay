#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <random>

template <typename T> T random(T min, T max, std::mt19937& random_generator)
{
    return std::conditional_t<std::is_integral<T>::value, std::uniform_int_distribution<T>,
        std::uniform_real_distribution<T>> { min, max }(random_generator);
}

template <typename T> T clamp(T value, T min, T max)
{
    if (value < min) {
        return min;
    }

    if (value > max) {
        return max;
    }

    return value;
}

bool threshold(float x, float y, float threshold);

float fade(float x, float y, float offset);

float sin_out_easing(float time, float start, float distance, float duration);
