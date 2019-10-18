#include "starship.hpp"

void Starship::update(std::shared_ptr<Controller::State> state)
{
    float dt = GetFrameTime();

    ship.controls = {
        IsKeyDown(KEY_W) || state->forward.load(),
        IsKeyDown(KEY_S),
        IsKeyDown(KEY_A) || state->left.load(),
        IsKeyDown(KEY_D) || state->right.load(),
        IsKeyDown(KEY_P),
        IsKeyDown(KEY_L),
    };

    ship.update(dt, map_size, columns);
}

void Starship::draw()
{
    BeginTextureMode(framebuffer);
    ClearBackground(BLACK);
    BeginMode3D(ship.camera.camera);

    DrawPlane(
        {
            0.0F,
            0.0F,
            map_size.z / 2.0F,
        },
        {
            map_size.x,
            map_size.z,
        },
        DARKBROWN);

    size_t columns_drawn {};
    bool first_column_drawn {};

    const float render_distance { 300.0F };

    for (auto& column : columns) {
        float distance = column.position.z - ship.position.z;

        if (distance < 0.0F) {
            continue;
        }

        if (distance < render_distance) {

            if (distance > render_distance * 0.5F) {
                column.color.a
                    = static_cast<unsigned char>((1.0F - ((distance / render_distance) - 0.5F) * 2.0F) * 255.0F);
            } else {
                column.color.a = 255;
            }

            column.draw();

            ++columns_drawn;
            first_column_drawn = true;

            if (columns_drawn % 100 == 0) {
                EndMode3D();
                BeginMode3D(ship.camera.camera);
            }
        } else if (first_column_drawn) {
            break;
        }
    }

    EndMode3D();
    EndTextureMode();
}

RenderTexture2D Starship::get_framebuffer()
{
    return framebuffer;
}
