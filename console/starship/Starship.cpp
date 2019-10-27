#include "Starship.hpp"

namespace Starship {

void Starship::update(std::shared_ptr<ControllerState> state)
{
    float dt = GetFrameTime();

    m_ship.set_controls({
        IsKeyDown(KEY_W) || state->buttons[3].load(),
        IsKeyDown(KEY_S) || state->buttons[4].load(),
        IsKeyDown(KEY_A) || state->roll.load() < -20,
        IsKeyDown(KEY_D) || state->roll.load() > 20,
        IsKeyDown(KEY_P) || state->pitch.load() > 20,
        IsKeyDown(KEY_L) || state->pitch.load() < -20,
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
