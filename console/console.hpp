#pragma once
#include "controller/controller.hpp"
#include "starship/starship.hpp"
#include "state.hpp"
#include <memory>

class Console {
private:
    std::unique_ptr<State> state;
    Controller controller;

public:
    Console(int width, int height)
    {
        InitWindow(width, height, "Raspberry Console");
        HideCursor();

        state = std::make_unique<Starship>();
    }

    void run();
};
