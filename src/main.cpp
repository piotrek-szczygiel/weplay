#include "raylib.h"

const int MAX_BUNNIES = 1000000;
const int MAX_BATCH_ELEMENTS = 8192;

struct Bunny {
    Vector2 position;
    Vector2 speed;
    Color color;
};

int main()
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(screenWidth, screenHeight, "rpi-tetris");

    auto texBunny = LoadTexture("resources/wabbit.png");

    auto bunnies = new Bunny[MAX_BUNNIES];
    int bunniesCount = 0;

    while (!WindowShouldClose()) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            if (bunniesCount < MAX_BUNNIES) {
                bunnies[bunniesCount].position = GetMousePosition();
                bunnies[bunniesCount].speed.x = static_cast<float>(GetRandomValue(-250, 250));
                bunnies[bunniesCount].speed.y = static_cast<float>(GetRandomValue(-250, 250));
                bunnies[bunniesCount].color = Color {
                    static_cast<unsigned char>(GetRandomValue(50, 240)),
                    static_cast<unsigned char>(GetRandomValue(80, 240)),
                    static_cast<unsigned char>(GetRandomValue(100, 240)),
                    255
                };
                bunniesCount++;
            }
        }

        auto dt = GetFrameTime();

        for (int i = 0; i < bunniesCount; i++) {
            bunnies[i].position.x += bunnies[i].speed.x * dt;
            bunnies[i].position.y += bunnies[i].speed.y * dt;

            if (((bunnies[i].position.x + texBunny.width / 2) > GetScreenWidth()) || ((bunnies[i].position.x + texBunny.width / 2) < 0))
                bunnies[i].speed.x *= -1;
            if (((bunnies[i].position.y + texBunny.height / 2) > GetScreenHeight()) || ((bunnies[i].position.y + texBunny.height / 2 - 40) < 0))
                bunnies[i].speed.y *= -1;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < bunniesCount; i++) {
            DrawTexture(texBunny, static_cast<int>(bunnies[i].position.x),
                static_cast<int>(bunnies[i].position.y), bunnies[i].color);
        }

        DrawRectangle(0, 0, screenWidth, 40, BLACK);
        DrawText(FormatText("bunnies: %i", bunniesCount), 120, 10, 20, GREEN);
        DrawText(FormatText("batched draw calls: %i",
                     1 + bunniesCount / MAX_BATCH_ELEMENTS),
            320, 10, 20, MAROON);

        DrawFPS(10, 10);
        EndDrawing();
    }

    delete[] bunnies;
    UnloadTexture(texBunny);
    CloseWindow();
    return 0;
}
