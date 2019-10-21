#include "Starship.hpp"

namespace Starship {

void Starship::update(std::shared_ptr<Controller::State> state)
{
    float dt = GetFrameTime();

    ship_.controls = {
        IsKeyDown(KEY_W) || state->forward.load(),
        IsKeyDown(KEY_S),
        IsKeyDown(KEY_A) || state->left.load(),
        IsKeyDown(KEY_D) || state->right.load(),
        IsKeyDown(KEY_P),
        IsKeyDown(KEY_L),
    };

    ship_.update(dt, mapSize_, columns_);
}

void Starship::draw()
{
    BeginTextureMode(framebuffer_);
    ClearBackground(BLACK);
    BeginMode3D(ship_.camera.camera);

    DrawPlane(
        {
            0.0F,
            0.0F,
            mapSize_.z / 2.0F,
        },
        {
            mapSize_.x,
            mapSize_.z,
        },
        DARKBROWN);

    size_t columnsDrawn {};
    bool firstColumnDrawn {};

    const float renderDistance { 300.0F };

    for (auto& column : columns_) {
        float distance = column.position.z - ship_.position.z;

        if (distance < 0.0F) {
            continue;
        }

        if (distance < renderDistance) {

            if (distance > renderDistance * 0.5F) {
                column.color.a
                    = static_cast<unsigned char>((1.0F - ((distance / renderDistance) - 0.5F) * 2.0F) * 255.0F);
            } else {
                column.color.a = 255;
            }

            column.draw();

            ++columnsDrawn;
            firstColumnDrawn = true;

            if (columnsDrawn % 100 == 0) {
                EndMode3D();
                BeginMode3D(ship_.camera.camera);
            }
        } else if (firstColumnDrawn) {
            break;
        }
    }

    EndMode3D();
    EndTextureMode();
}

RenderTexture2D Starship::getFramebuffer()
{
    return framebuffer_;
}

}
