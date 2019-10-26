#include "Console.hpp"
#include "pong/Pong.hpp"
#include "starship/Starship.hpp"
#include "tetris/Tetris.hpp"
#include <algorithm>
#include <raylib.h>

void Console::run()
{
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F1)) {
            m_current_state = std::make_unique<Menu::Menu>();
        } else if (IsKeyPressed(KEY_F2)) {
            m_current_state = std::make_unique<Starship::Starship>();
        } else if (IsKeyPressed(KEY_F3)) {
            m_current_state = std::make_unique<Tetris::Tetris>();
        } else if (IsKeyPressed(KEY_F4)) {
            m_current_state = std::make_unique<Pong::Pong>();
        }

        m_current_state->update(m_controller.state);
        m_current_state->draw();

        BeginDrawing();
        ClearBackground(BLACK);

        auto width { static_cast<float>(m_current_state->framebuffer().texture.width) };
        auto height { static_cast<float>(m_current_state->framebuffer().texture.height) };
        auto screen_width { static_cast<float>(GetScreenWidth()) };
        auto screen_height { static_cast<float>(GetScreenHeight()) };
        float scale { std::min(screen_width / width, screen_height / height) };

        RlRectangle source {
            0.0F,
            0.0F,
            width,
            -height,
        };

        RlRectangle dest {
            (screen_width - width * scale) / 2.0F,
            (screen_height - height * scale) / 2.0F,
            width * scale,
            height * scale,
        };

        DrawTexturePro(m_current_state->framebuffer().texture, source, dest, {}, 0.0F, WHITE);

        DrawFPS(10, 10);
        EndDrawing();
    }
}
