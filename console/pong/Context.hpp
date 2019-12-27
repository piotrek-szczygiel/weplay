#pragma once

#include <array>
#include <raylib.h>

namespace Pong {

class Context {
public:
    Texture2D power_up_textures;

    static Context& instance()
    {
        static Context instance;
        return instance;
    }

    ~Context() { UnloadTexture(power_up_textures); }

private:
    Context() { power_up_textures = LoadTexture("resources/pong/power_ups.png"); }
};

}
