#pragma once
#include <random>
#include <raylib.h>

namespace Tetris {

class Context {
public:
    std::mt19937 rng;

    Texture2D tiles;
    Texture2D tiles_small;

    Texture2D texture_bg;
    Texture2D texture_ui;

    Font font;

    static Context& instance()
    {
        static Context instance;
        return instance;
    }

    ~Context()
    {
        UnloadTexture(tiles);
        UnloadTexture(tiles_small);
        UnloadTexture(texture_bg);
        UnloadTexture(texture_ui);
        UnloadFont(font);
    }

private:
    Context()
        : rng { std::random_device {}() }
        , tiles { LoadTexture("resources/tetris/tiles.png") }
        , tiles_small { LoadTexture("resources/tetris/tiles_small.png") }
        , texture_bg { LoadTexture("resources/tetris/background.png") }
        , texture_ui { LoadTexture("resources/tetris/ui.png") }
        , font { LoadFont("resources/tetris/font.ttf") }
    {
    }
};

}