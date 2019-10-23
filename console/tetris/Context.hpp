#pragma once
#include <random>
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

    std::mt19937& rng() { return m_gen; }

    Texture2D tiles() { return m_tiles; }
    Texture2D tiles_small() { return m_tiles_small; }

private:
    Context()
        : m_tiles { LoadTexture("resources/tetris/tiles.png") }
        , m_tiles_small { LoadTexture("resources/tetris/tiles_small.png") }
    {
    }

    std::mt19937 m_gen { std::random_device {}() };

    Texture2D m_tiles;
    Texture2D m_tiles_small;
};

}