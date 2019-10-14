#include "Console.hpp"
#include "Starship.hpp"
#include "raylib.h"
#include <algorithm>

Console::Console() { currentState = std::make_unique<Starship>(); }

void Console::Initialize()
{
    SetConfigFlags(static_cast<unsigned int>(FLAG_VSYNC_HINT) | static_cast<unsigned int>(FLAG_FULLSCREEN_MODE));
    InitWindow(3000, 2000, "Raspberry Console");

    currentState->Initialize();
}

void Console::Run()
{
    while (!WindowShouldClose()) {
        currentState->Update();
        currentState->Draw();

        BeginDrawing();
        ClearBackground(WHITE);

        auto gw = static_cast<float>(currentState->GetFramebuffer().texture.width);
        auto gh = static_cast<float>(currentState->GetFramebuffer().texture.height);
        auto sw = static_cast<float>(GetScreenWidth());
        auto sh = static_cast<float>(GetScreenHeight());
        float scale = std::min(sw / gw, sh / gh);

        Rectangle source = {
            0.0F,
            0.0F,
            gw,
            -gh,
        };

        Rectangle dest = {
            (sw - gw * scale) / 2.0F,
            (sh - gh * scale) / 2.0F,
            gw * scale,
            gh * scale,
        };

        DrawTexturePro(currentState->GetFramebuffer().texture, source, dest, Vector2 {}, 0.0F, WHITE);

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
}
