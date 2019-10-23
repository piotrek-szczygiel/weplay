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
    Texture2D texture_ui() { return m_texture_ui; }

    Font font() { return m_font; }

private:
    Context()
        : m_tiles { LoadTexture("resources/tetris/tiles.png") }
        , m_tiles_small { LoadTexture("resources/tetris/tiles_small.png") }
        , m_texture_ui { LoadTexture("resources/tetris/ui.png") }
        , m_font { LoadFont("resources/tetris/font.ttf") }
    {
    }

    std::mt19937 m_gen { std::random_device {}() };

    Texture2D m_tiles;
    Texture2D m_tiles_small;
    Texture2D m_texture_ui;

    Font m_font;
};

}