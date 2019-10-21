#include "Console.hpp"
#include "controller/Controller.hpp"
#include "starship/Starship.hpp"
#include "tetris/Tetris.hpp"
#include <algorithm>
#include <raylib.h>

void Console::run()
{
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_F1)) {
            currentState_ = std::make_unique<Menu::Menu>();
        } else if (IsKeyPressed(KEY_F2)) {
            currentState_ = std::make_unique<Starship::Starship>();
        } else if (IsKeyPressed(KEY_F3)) {
            currentState_ = std::make_unique<Tetris::Tetris>();
        }

        currentState_->update(controller_.state);
        currentState_->draw();

        BeginDrawing();
        ClearBackground(WHITE);

        auto gameWidth { static_cast<float>(currentState_->getFramebuffer().texture.width) };
        auto gameHeight { static_cast<float>(currentState_->getFramebuffer().texture.height) };
        auto screenWidth { static_cast<float>(GetScreenWidth()) };
        auto screenHeight { static_cast<float>(GetScreenHeight()) };
        float scale { std::min(screenWidth / gameWidth, screenHeight / gameHeight) };

        RlRectangle source {
            0.0F,
            0.0F,
            gameWidth,
            -gameHeight,
        };

        RlRectangle dest {
            (screenWidth - gameWidth * scale) / 2.0F,
            (screenHeight - gameHeight * scale) / 2.0F,
            gameWidth * scale,
            gameHeight * scale,
        };

        DrawTexturePro(currentState_->getFramebuffer().texture, source, dest, Vector2 {}, 0.0F, WHITE);

        DrawFPS(10, 10);
        EndDrawing();
    }
}
