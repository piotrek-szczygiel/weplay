#pragma once
#include <random>

extern thread_local std::mt19937 gen;

template <typename T> T Random(T min, T max)
{

    using dist = std::conditional_t<std::is_integral<T>::value, std::uniform_int_distribution<T>,
        std::uniform_real_distribution<T>>;
    return dist { min, max }(gen);
}

bool Between(float x, float y, float threshold);

float Fade(float x, float y, float offset);