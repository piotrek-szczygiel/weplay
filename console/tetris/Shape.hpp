#pragma once
#include <array>
#include <raylib.h>

namespace Tetris {

using ShapeType = int;

constexpr ShapeType I = 1;
constexpr ShapeType J = 2;
constexpr ShapeType L = 3;
constexpr ShapeType O = 4;
constexpr ShapeType S = 5;
constexpr ShapeType T = 6;
constexpr ShapeType Z = 7;

struct Kick {
    int x;
    int y;
};

struct RotationKick {
    std::array<Kick, 4> counterClockwise;
    std::array<Kick, 4> clockwise;
};

using ShapeKicks = std::array<RotationKick, 4>;

using SingleGrid = std::array<std::array<ShapeType, 4>, 4>;

struct ShapeGrid {
    int offsetX;
    int offsetY;
    int width;
    int height;
    SingleGrid grid;
};

struct Shape {
    ShapeType type;
    ShapeKicks kicks;
    std::array<ShapeGrid, 4> grids;

    void draw(int drawX, int drawY, int rotation) const;
};

constexpr ShapeKicks KICKS_JLOSTZ { {
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

constexpr ShapeKicks KICKS_I { {
    {
        { { { -2, 0 }, { 1, 0 }, { -2, 1 }, { 1, -2 } } },
        { { { -1, 0 }, { 2, 0 }, { -1, -2 }, { 2, 1 } } },
    },
    {
        { { { -1, 0 }, { 2, 0 }, { -1, -2 }, { 2, 1 } } },
        { { { 2, 0 }, { -1, 0 }, { 2, -1 }, { -1, 2 } } },
    },
    {
        { { { 2, 0 }, { -1, 0 }, { 2, -1 }, { -1, 2 } } },
        { { { 1, 0 }, { -2, 0 }, { 1, 2 }, { -2, -1 } } },
    },
    {
        { { { 1, 0 }, { -2, 0 }, { 1, 2 }, { -2, -1 } } },
        { { { -2, 0 }, { 1, 0 }, { -2, 1 }, { 1, -2 } } },
    },
} };

constexpr Shape SHAPE_I {
    I,
    KICKS_I,
    { {
        { 0, 1, 4, 1, { { { { 0, 0, 0, 0 } }, { { I, I, I, I } }, { { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 2, 0, 1, 4, { { { { 0, 0, I, 0 } }, { { 0, 0, I, 0 } }, { { 0, 0, I, 0 } }, { { 0, 0, I, 0 } } } } },
        { 0, 2, 4, 1, { { { { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 } }, { { I, I, I, I } }, { { 0, 0, 0, 0 } } } } },
        { 1, 0, 1, 4, { { { { 0, I, 0, 0 } }, { { 0, I, 0, 0 } }, { { 0, I, 0, 0 } }, { { 0, I, 0, 0 } } } } },
    } },
};

constexpr Shape SHAPE_J {
    J,
    KICKS_JLOSTZ,
    { {
        { 0, 0, 3, 2, { { { { J, 0, 0, 0 } }, { { J, J, J, 0 } }, { { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 1, 0, 2, 3, { { { { 0, J, J, 0 } }, { { 0, J, 0, 0 } }, { { 0, J, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 1, 3, 2, { { { { 0, 0, 0, 0 } }, { { J, J, J, 0 } }, { { 0, 0, J, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 0, 2, 3, { { { { 0, J, 0, 0 } }, { { 0, J, 0, 0 } }, { { J, J, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
    } },
};

constexpr Shape SHAPE_L {
    L,
    KICKS_JLOSTZ,
    { {
        { 0, 0, 3, 2, { { { { 0, 0, L, 0 } }, { { L, L, L, 0 } }, { { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 1, 0, 2, 3, { { { { 0, L, 0, 0 } }, { { 0, L, 0, 0 } }, { { 0, L, L, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 1, 3, 2, { { { { 0, 0, 0, 0 } }, { { L, L, L, 0 } }, { { L, 0, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 0, 2, 3, { { { { L, L, 0, 0 } }, { { 0, L, 0, 0 } }, { { 0, L, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
    } },
};

constexpr Shape SHAPE_O {
    O,
    KICKS_JLOSTZ,
    { {
        { 0, 0, 2, 2, { { { { O, O, 0, 0 } }, { { O, O, 0, 0 } }, { { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 0, 2, 2, { { { { O, O, 0, 0 } }, { { O, O, 0, 0 } }, { { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 0, 2, 2, { { { { O, O, 0, 0 } }, { { O, O, 0, 0 } }, { { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 0, 2, 2, { { { { O, O, 0, 0 } }, { { O, O, 0, 0 } }, { { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
    } },
};

constexpr Shape SHAPE_S {
    S,
    KICKS_JLOSTZ,
    { {
        { 0, 0, 3, 2, { { { { 0, S, S, 0 } }, { { S, S, 0, 0 } }, { { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 1, 0, 2, 3, { { { { 0, S, 0, 0 } }, { { 0, S, S, 0 } }, { { 0, 0, S, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 1, 3, 2, { { { { 0, 0, 0, 0 } }, { { 0, S, S, 0 } }, { { S, S, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 0, 2, 3, { { { { S, 0, 0, 0 } }, { { S, S, 0, 0 } }, { { 0, S, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
    } },
};

constexpr Shape SHAPE_T {
    T,
    KICKS_JLOSTZ,
    { {
        { 0, 0, 3, 2, { { { { 0, T, 0, 0 } }, { { T, T, T, 0 } }, { { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 1, 0, 2, 3, { { { { 0, T, 0, 0 } }, { { 0, T, T, 0 } }, { { 0, T, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 1, 3, 2, { { { { 0, 0, 0, 0 } }, { { T, T, T, 0 } }, { { 0, T, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 0, 2, 3, { { { { 0, T, 0, 0 } }, { { T, T, 0, 0 } }, { { 0, T, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
    } },
};

constexpr Shape SHAPE_Z {
    Z,
    KICKS_JLOSTZ,
    { {
        { 0, 0, 3, 2, { { { { Z, Z, 0, 0 } }, { { 0, Z, Z, 0 } }, { { 0, 0, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 1, 0, 2, 3, { { { { 0, 0, Z, 0 } }, { { 0, Z, Z, 0 } }, { { 0, Z, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 1, 3, 2, { { { { 0, 0, 0, 0 } }, { { Z, Z, 0, 0 } }, { { 0, Z, Z, 0 } }, { { 0, 0, 0, 0 } } } } },
        { 0, 0, 2, 3, { { { { 0, Z, 0, 0 } }, { { Z, Z, 0, 0 } }, { { Z, 0, 0, 0 } }, { { 0, 0, 0, 0 } } } } },
    } },
};

constexpr std::array<Shape, 7> SHAPES_ALL { { SHAPE_I, SHAPE_J, SHAPE_L, SHAPE_O, SHAPE_S, SHAPE_T, SHAPE_Z } };

constexpr std::array<Color, 8> SHAPE_COLORS { {
    BLACK,
    SKYBLUE,
    BLUE,
    ORANGE,
    YELLOW,
    GREEN,
    PURPLE,
    RED,
} };

}