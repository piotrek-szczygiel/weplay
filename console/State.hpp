#pragma once
#include "controller/Controller.hpp"
#include <raylib.h>

class State {
public:
    State() = default;

    State(const State&) = delete;
    State& operator=(const State&) = delete;

    State(State&&) = delete;
    State& operator=(State&&) = delete;

    virtual ~State() = default;

    virtual void update(std::shared_ptr<ControllerState> state) = 0;
    virtual void draw() = 0;
    virtual RenderTexture2D framebuffer() = 0;
    virtual bool exit() = 0;
};
