#include "console.hpp"
#include "controller/controller.hpp"
#include "starship/starship.hpp"
#include "tetris/tetris.hpp"
#include <algorithm>
#include <raylib.h>

void Console::run()
{
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F1)) {
            current_state = std::make_unique<Menu>();
        } else if (IsKeyPressed(KEY_F2)) {
            current_state = std::make_unique<Starship>();
        } else if (IsKeyPressed(KEY_F3)) {
            current_state = std::make_unique<Tetris>();
        }

        current_state->update(controller.state);
        current_state->draw();

        BeginDrawing();
        ClearBackground(WHITE);

        auto game_width { static_cast<float>(current_state->get_framebuffer().texture.width) };
        auto game_height { static_cast<float>(current_state->get_framebuffer().texture.height) };
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

        DrawTexturePro(current_state->get_framebuffer().texture, source, dest, Vector2 {}, 0.0F, WHITE);

        DrawFPS(10, 10);
        EndDrawing();
    }
}
