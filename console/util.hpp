#pragma once
#include <random>

extern thread_local std::mt19937 random_generator;

template <typename T> T random(T min, T max)
{

    using dist = std::conditional_t<std::is_integral<T>::value, std::uniform_int_distribution<T>,
        std::uniform_real_distribution<T>>;
    return dist { min, max }(random_generator);
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