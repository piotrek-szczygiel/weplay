#include <algorithm>
#include <cstdio>
#include <fmt/core.h>
#include <raylib.h>
#include <vector>

const int MAX_BUNNIES = 1000000;

struct Bunny {
    Vector2 position;
    Vector2 speed;
    Color color;

    static Bunny Random(int maxX, int maxY)
    {
        return { { static_cast<float>(GetRandomValue(0, maxX)),
                     static_cast<float>(GetRandomValue(0, maxY)) },
            { static_cast<float>(GetRandomValue(-250, 250)),
                static_cast<float>(GetRandomValue(-250, 250)) },
            { static_cast<unsigned char>(GetRandomValue(50, 240)),
                static_cast<unsigned char>(GetRandomValue(80, 240)),
                static_cast<unsigned char>(GetRandomValue(100, 240)), 255 } };
    }
};

int main()
{
    const int minWindowWidth = 640;
    const int minWindowHeight = 360;

    const int gameWidth = 1920;
    const int gameHeight = 1080;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(1600, 900, "Raspberry Console");
    SetWindowMinSize(minWindowWidth, minWindowHeight);

    RenderTexture2D target = LoadRenderTexture(gameWidth, gameHeight);
    SetTextureFilter(target.texture, FILTER_BILINEAR);

    auto texBunny = LoadTexture("resources/wabbit.png");

    auto bunnies = std::vector<Bunny> {};

    while (!WindowShouldClose()) {
        float scale = std::min(static_cast<float>(GetScreenWidth()) / gameWidth,
            static_cast<float>(GetScreenHeight()) / gameHeight);

        if (IsKeyDown(KEY_Q)) {
            break;
        }

        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }

        if (IsKeyDown(KEY_SPACE) && bunnies.size() < MAX_BUNNIES) {
            bunnies.push_back(Bunny::Random(gameWidth, gameHeight));
        }

        auto dt = GetFrameTime();

        for (auto& bunny : bunnies) {
            bunny.position.x += bunny.speed.x * dt;
            bunny.position.y += bunny.speed.y * dt;

            if (((static_cast<int>(bunny.position.x) + texBunny.width / 2) > GetScreenWidth())
                || ((static_cast<int>(bunny.position.x) + texBunny.width / 2) < 0)) {
                bunny.speed.x *= -1;
            }
            if (((static_cast<int>(bunny.position.y) + texBunny.height / 2) > GetScreenHeight())
                || ((static_cast<int>(bunny.position.y) + texBunny.height / 2 - 40) < 0)) {
                bunny.speed.y *= -1;
            }
        }

        BeginDrawing();
        BeginTextureMode(target);
        ClearBackground(RAYWHITE);

        for (const auto& bunny : bunnies) {
            DrawTexture(texBunny, static_cast<int>(bunny.position.x),
                static_cast<int>(bunny.position.y), bunny.color);
        }

        DrawRectangle(0, 0, gameWidth, 40, LIGHTGRAY);
        DrawText(fmt::format("bunnies: {}", bunnies.size()).c_str(), 120, 10, 20, GREEN);

        DrawFPS(10, 10);
        EndTextureMode();
        DrawTexturePro(target.texture,
            { 0.0F, 0.0F, static_cast<float>(target.texture.width),
                static_cast<float>(-target.texture.height) },
            { (static_cast<float>(GetScreenWidth()) - (static_cast<float>(gameWidth) * scale))
                    * 0.5F,
                (static_cast<float>(GetScreenHeight()) - (static_cast<float>(gameHeight) * scale))
                    * 0.5F,
                static_cast<float>(gameWidth) * scale, static_cast<float>(gameHeight) * scale },
            { 0, 0 }, 0.0F, WHITE);
        EndDrawing();
    }

    UnloadTexture(texBunny);
    CloseWindow();
    return 0;
}
