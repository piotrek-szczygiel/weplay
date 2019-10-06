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

  static auto Random(int maxX, int maxY) -> Bunny
  {
    return { { static_cast<float>(GetRandomValue(0, maxX)), static_cast<float>(GetRandomValue(0, maxY)) },
      { static_cast<float>(GetRandomValue(-250, 250)), static_cast<float>(GetRandomValue(-250, 250)) },
      { static_cast<unsigned char>(GetRandomValue(50, 240)), static_cast<unsigned char>(GetRandomValue(80, 240)),
          static_cast<unsigned char>(GetRandomValue(100, 240)), 255 } };
  }
};

auto main() -> int
{
  const int gameWidth = 960;
  const int gameHeight = 540;

  SetConfigFlags(FLAG_FULLSCREEN_MODE);
  InitWindow(3000, 2000, "Raspberry Console");

  RenderTexture2D renderTarget = LoadRenderTexture(gameWidth, gameHeight);
  SetTextureFilter(renderTarget.texture, FILTER_POINT);

  Texture2D texBunny = LoadTexture("resources/wabbit.png");

  std::vector<Bunny> bunnies;

  while (!WindowShouldClose()) {
    if (IsKeyDown(KEY_Q)) {
      break;
    }

    if (IsKeyDown(KEY_SPACE) && bunnies.size() < MAX_BUNNIES) {
      bunnies.push_back(Bunny::Random(gameWidth, gameHeight));
    }

    float dt = GetFrameTime();

    for (auto& bunny : bunnies) {
      bunny.position.x += bunny.speed.x * dt;
      bunny.position.y += bunny.speed.y * dt;

      if (((static_cast<int>(bunny.position.x) + texBunny.width / 2) > gameWidth)
          || ((static_cast<int>(bunny.position.x) + texBunny.width / 2) < 0)) {
        bunny.speed.x *= -1;
      }
      if (((static_cast<int>(bunny.position.y) + texBunny.height / 2) > gameHeight)
          || ((static_cast<int>(bunny.position.y) + texBunny.height / 2 - 40) < 0)) {
        bunny.speed.y *= -1;
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);
    BeginTextureMode(renderTarget);
    ClearBackground(RAYWHITE);

    for (const auto& bunny : bunnies) {
      DrawTexture(texBunny, static_cast<int>(bunny.position.x), static_cast<int>(bunny.position.y), bunny.color);
    }

    DrawRectangle(0, 0, gameWidth, 40, LIGHTGRAY);
    DrawText(fmt::format("bunnies: {}", bunnies.size()).c_str(), 120, 10, 20, GREEN);

    DrawFPS(10, 10);
    EndTextureMode();

    // Draw scaled render target onto the screen
    float scale = std::min(
        static_cast<float>(GetScreenWidth()) / gameWidth, static_cast<float>(GetScreenHeight()) / gameHeight);

    DrawTexturePro(renderTarget.texture,
        { 0.0F, 0.0F, static_cast<float>(renderTarget.texture.width),
            static_cast<float>(-renderTarget.texture.height) },
        { (static_cast<float>(GetScreenWidth()) - (static_cast<float>(gameWidth) * scale)) * 0.5F,
            (static_cast<float>(GetScreenHeight()) - (static_cast<float>(gameHeight) * scale)) * 0.5F,
            static_cast<float>(gameWidth) * scale, static_cast<float>(gameHeight) * scale },
        { 0, 0 }, 0.0F, WHITE);

    EndDrawing();
  }

  UnloadTexture(texBunny);
  CloseWindow();
  return 0;
}
