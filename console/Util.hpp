#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <random>
#include <raylib.h>

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

void draw_scaled(RenderTexture2D framebuffer);
