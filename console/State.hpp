#pragma once
#include "raylib.h"

class State {
public:
    State() = default;

    State(const State&) = delete;
    State& operator=(const State&) = delete;

    State(State&&) = delete;
    State& operator=(State&&) = delete;

    virtual ~State() = default;
    virtual void Initialize() = 0;
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual RenderTexture2D GetFramebuffer() = 0;
};
