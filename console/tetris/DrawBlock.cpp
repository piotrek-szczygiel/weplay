#include "DrawBlock.hpp"
#include "Context.hpp"
#include <cassert>
#include <raylib.h>

namespace Tetris {

void DrawBlock(ShapeType type, int game_level, int x, int y, unsigned char alpha, bool small)
{
    // Quick hack to clip drawing blocks above matrix
    if (y < 48) {
        return;
    }

    int tile_size_int = small ? TILE_SIZE_SMALL : TILE_SIZE;

    if (type == 0) {
        // DrawRectangle(x - 1, y, tile_size_int + 1, tile_size_int, BLACK);
        return;
    }

    auto tile_size = static_cast<float>(tile_size_int);
    auto tiles = small ? Context::instance().tiles_small : Context::instance().tiles;

    int index = static_cast<int>(type) - 1;
    assert(index >= 0);
    assert(index <= 7);

    int level = game_level - 1;
    assert(level >= 0);
    assert(level <= 9);

    RlRectangle source {
        static_cast<float>(index) * tile_size,
        static_cast<float>(level) * tile_size,
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
