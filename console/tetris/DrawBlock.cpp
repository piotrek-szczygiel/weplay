#include "DrawBlock.hpp"
#include "Context.hpp"
#include <cassert>
#include <raylib.h>

namespace Tetris {

void DrawBlock(ShapeType type, int x, int y, unsigned char alpha, bool small)
{
    constexpr float TILE_SIZE = 8.0F;
    constexpr float TILE_SIZE_SMALL = 6.0F;

    auto tiles = small ? Context::instance().tiles_small() : Context::instance().tiles();
    auto tile_size = small ? TILE_SIZE_SMALL : TILE_SIZE;

    int index = static_cast<int>(type) - 1;
    assert(index >= 0);
    assert(index <= 7);

    RlRectangle source {
        static_cast<float>(index) * tile_size,
        0.0F,
        tile_size,
        tile_size,
    };

    Vector2 position {
        static_cast<float>(x),
        static_cast<float>(y),
    };

    Color color { 255, 255, 255, alpha };

    DrawTextureRec(tiles, source, position, color);
}

}