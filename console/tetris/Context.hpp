#pragma once
#include <raylib.h>

namespace Tetris {

class Context {
public:
    static Context& instance()
    {
        static Context instance;
        return instance;
    }

    ~Context() { UnloadTexture(m_tiles); }

    Texture2D tiles() { return m_tiles; }
    Texture2D tiles_small() { return m_tiles_small; }

private:
    Context()
        : m_tiles { LoadTexture("resources/tetris/tiles.png") }
        , m_tiles_small { LoadTexture("resources/tetris/tiles_small.png") }
    {
    }

    Texture2D m_tiles;
    Texture2D m_tiles_small;
};

}