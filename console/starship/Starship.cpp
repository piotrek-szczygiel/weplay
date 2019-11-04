#include "Starship.hpp"
#include "../Util.hpp"

namespace Starship {

void Starship::update(std::shared_ptr<ControllerState> state)
{
    if (IsKeyPressed(KEY_Q) || state->buttons[8]) {
        m_exit = true;
        return;
    }

    float dt = GetFrameTime();

    float roll {};

    if (IsKeyDown(KEY_A)) {
        roll = -1.0F;
    } else if (IsKeyDown(KEY_D)) {
        roll = 1.0F;
    } else {
        if (state->roll >= -10 && state->roll <= 10) {
            roll = 0.0F;
        } else if (state->roll < 0) {
            roll = static_cast<float>(state->roll + 10) / 20.0F;
        } else {
            roll = static_cast<float>(state->roll - 10) / 20.0F;
        }

        roll = clamp(roll, -1.0F, 1.0F);
    }

    m_ship.set_controls({
        IsKeyDown(KEY_W) || state->buttons[0],
        IsKeyDown(KEY_S) || state->buttons[1],
        IsKeyDown(KEY_P) || state->buttons[4],
        IsKeyDown(KEY_L) || state->buttons[5],
        roll,
    });

    m_ship.update(dt, m_map_size, m_columns);
}

void Starship::draw()
{
    constexpr float RENDER_DISTANCE { 300.0F };

    BeginTextureMode(m_framebuffer);
    ClearBackground(BLACK);
    BeginMode3D(m_ship.camera());

    DrawPlane(
        {
            0.0F,
            0.0F,
            m_map_size.z / 2.0F,
        },
        {
            m_map_size.x,
            m_map_size.z,
        },
        DARKBROWN);

    size_t columns_drawn {};
    bool drawn_first_column {};

    for (auto& column : m_columns) {
        float distance = column.position().z - m_ship.position().z;

        if (distance < 0.0F) {
            continue;
        }

        if (distance < RENDER_DISTANCE) {

            if (distance > RENDER_DISTANCE * 0.5F) {
                column.set_alpha(static_cast<unsigned char>(
                    (1.0F - ((distance / RENDER_DISTANCE) - 0.5F) * 2.0F) * 255.0F));
            } else {
                column.set_alpha(255);
            }

            column.draw();

            ++columns_drawn;
            drawn_first_column = true;

            if (columns_drawn % 100 == 0) {
                EndMode3D();
                BeginMode3D(m_ship.camera());
            }
        } else if (drawn_first_column) {
            break;
        }
    }

    EndMode3D();
    EndTextureMode();
}

RenderTexture2D Starship::framebuffer() { return m_framebuffer; }

}
