#include "Console.h"
#include "Starship.h"
#include "raylib.h"
#include <algorithm>

Console::Console() { currentState = std::make_unique<Starship>(); }

void Console::Initialize()
{
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_FULLSCREEN_MODE);
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

        float gw = currentState->GetFramebuffer().texture.width;
        float gh = currentState->GetFramebuffer().texture.height;
        float sw = static_cast<float>(GetScreenWidth());
        float sh = static_cast<float>(GetScreenHeight());
        float scale = std::min(sw / gw, sh / gh);

        Rectangle source = { 0.0F, 0.0F, gw, -gh };
        Rectangle dest = { (sw - gw * scale) / 2.0F, (sh - gh * scale) / 2.0F, gw * scale, gh * scale };
        DrawTexturePro(currentState->GetFramebuffer().texture, source, dest, Vector2 {}, 0.0F, WHITE);

		DrawFPS(10, 10);
		EndDrawing();
    }

	CloseWindow();
}
