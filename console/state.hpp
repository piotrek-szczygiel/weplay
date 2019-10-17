#pragma once
#include "controller/controller.hpp"
#include <raylib.h>

class State {
public:
    State() = default;

    State(const State&) = delete;
    State& operator=(const State&) = delete;

    State(State&&) = delete;
    State& operator=(State&&) = delete;

    virtual ~State() = default;

    virtual void init() = 0;
    virtual void update(const std::vector<Event_Key>&) = 0;
    virtual void draw() = 0;
    virtual RenderTexture2D get_framebuffer() = 0;
};
