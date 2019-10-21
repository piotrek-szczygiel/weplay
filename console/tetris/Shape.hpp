#pragma once
#include <array>

struct Kick {
    int x;
    int y;
};

struct RotationKick {
    std::array<Kick, 4> counterClockwise;
    std::array<Kick, 4> clockwise;
};

using ShapeKicks = std::array<RotationKick, 4>;

constexpr ShapeKicks kicksJLSTZ { {
    {
        { { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } } },
        { { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } } },
    },
    {
        { { { 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 } } },
        { { { 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 } } },
    },
    {
        { { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } } },
        { { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } } },
    },
    {
        { { { -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 } } },
        { { { -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 } } },
    },
} };
