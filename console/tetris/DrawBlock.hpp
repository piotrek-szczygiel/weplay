#pragma once
#include "Shape.hpp"

namespace Tetris {

constexpr int TILE_SIZE { 8 };
constexpr int TILE_SIZE_SMALL { 6 };

void DrawBlock(ShapeType type, int level, int x, int y, unsigned char alpha, bool small);

}
