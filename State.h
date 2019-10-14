#pragma once
#include "raylib.h"

class State {
public:
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual RenderTexture2D GetFramebuffer() = 0;
};

