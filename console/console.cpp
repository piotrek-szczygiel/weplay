#include "console.hpp"
#include "controller/controller.hpp"
#include "starship/starship.hpp"
#include <algorithm>
#include <raylib.h>

Console::Console() { current_state = std::make_unique<Starship>(); }

void Console::init()
{
    InitWindow(1280, 720, "Raspberry Console");
    HideCursor();
    current_state->init();
}

void Console::run()
{
    while (!WindowShouldClose()) {
        current_state->update(controller.state);
        current_state->draw();

        BeginDrawing();
        ClearBackground(WHITE);

        auto gw = static_cast<float>(current_state->get_framebuffer().texture.width);
        auto gh = static_cast<float>(current_state->get_framebuffer().texture.height);
        auto sw = static_cast<float>(GetScreenWidth());
        auto sh = static_cast<float>(GetScreenHeight());
        float scale = std::min(sw / gw, sh / gh);

        RlRectangle source = {
            0.0F,
            0.0F,
            gw,
            -gh,
        };

        RlRectangle dest = {
            (sw - gw * scale) / 2.0F,
            (sh - gh * scale) / 2.0F,
            gw * scale,
            gh * scale,
        };

        DrawTexturePro(
            current_state->get_framebuffer().texture, source, dest, Vector2 {}, 0.0F, WHITE);

        DrawFPS(10, 10);
        EndDrawing();
    }

    RlCloseWindow();
}
