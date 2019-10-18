#include "console.hpp"
#include "controller/controller.hpp"
#include "starship/starship.hpp"
#include <algorithm>
#include <raylib.h>

void Console::run()
{
    while (!WindowShouldClose()) {
        state->update(controller.state);
        state->draw();

        BeginDrawing();
        ClearBackground(WHITE);

        auto game_width { static_cast<float>(state->get_framebuffer().texture.width) };
        auto game_height { static_cast<float>(state->get_framebuffer().texture.height) };
        auto screen_width { static_cast<float>(GetScreenWidth()) };
        auto screen_height { static_cast<float>(GetScreenHeight()) };
        float scale { std::min(screen_width / game_width, screen_height / game_height) };

        RlRectangle source {
            0.0F,
            0.0F,
            game_width,
            -game_height,
        };

        RlRectangle dest {
            (screen_width - game_width * scale) / 2.0F,
            (screen_height - game_height * scale) / 2.0F,
            game_width * scale,
            game_height * scale,
        };

        DrawTexturePro(state->get_framebuffer().texture, source, dest, Vector2 {}, 0.0F, WHITE);

        DrawFPS(10, 10);
        EndDrawing();
    }

    RlCloseWindow();
}
