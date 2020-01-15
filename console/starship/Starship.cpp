#include "Starship.hpp"
#include "../Util.hpp"
#include <raymath.h>

namespace Starship {

void Starship::update(const std::vector<ControllerState>& controllers)
{
    for (int i = 0; i < 2; ++i) {
        auto& player = (i == 0) ? m_player_1 : m_player_2;
        auto& controller = controllers[i];

        if (controller.buttons[8]) {
            m_state_change = StateChange::Menu;
            return;
        }

        player.set_controls({
            controller.buttons[0],
            controller.buttons[1],
            controller.buttons[4],
            controller.buttons[5],
            clamp(static_cast<float>(controller.roll) / 45.0F, -1.0F, 1.0F),
        });
    }

    // m_player_1.set_controls({
    //    IsKeyDown(KEY_UP),
    //    IsKeyDown(KEY_DOWN),
    //    IsKeyDown(KEY_Z),
    //    IsKeyDown(KEY_X),
    //    IsKeyDown(KEY_LEFT) ? -1.0F : IsKeyDown(KEY_RIGHT) ? 1.0F : 0.0F,
    //});

    float dt = GetFrameTime();
    m_player_1.update(dt, m_map_size, m_columns);
    m_player_2.update(dt, m_map_size, m_columns);

    if (m_state == StarshipState::RACING) {
        if (m_player_1.position().z >= m_map_size.z) {
            m_state = StarshipState::PLAYER_1_WON;
        } else if (m_player_2.position().z >= m_map_size.z) {
            m_state = StarshipState::PLAYER_2_WON;
        }
    }
}

void Starship::draw()
{
    for (int i = 0; i < 2; ++i) {
        auto& view = (i == 0) ? m_view_1 : m_view_2;
        auto& player = (i == 0) ? m_player_1 : m_player_2;
        auto& opponent = (i == 0) ? m_player_2 : m_player_1;

        BeginTextureMode(view);
        ClearBackground(BLACK);
        BeginMode3D(player.camera());

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
            float distance = column.position().z - player.position().z;

            if (distance < 0.0F) {
                continue;
            }

            if (distance < m_render_distance) {

                if (distance > m_render_distance * 0.5F) {
                    column.set_alpha(static_cast<unsigned char>(
                        (1.0F - ((distance / m_render_distance) - 0.5F) * 2.0F) * 255.0F));
                } else {
                    column.set_alpha(255);
                }

                column.draw();

                ++columns_drawn;
                drawn_first_column = true;

                if (columns_drawn % 100 == 0) {
                    EndMode3D();
                    BeginMode3D(player.camera());
                }
            } else if (drawn_first_column) {
                break;
            }
        }

        EndMode3D();
        EndTextureMode();
    }

    DrawTexturePro(m_view_1.texture, m_view_source, m_view_dest_1, {}, 0.0F, WHITE);
    DrawTexturePro(m_view_2.texture, m_view_source, m_view_dest_2, {}, 0.0F, WHITE);

    if (m_state != StarshipState::RACING) {
        int font_size = m_height / 10;

        int y1 = m_height / 4 - font_size / 2;
        int y2 = m_height / 4 * 3 - font_size / 2;

        int p1 = m_state == StarshipState::PLAYER_1_WON ? y1 : y2;
        int p2 = m_state == StarshipState::PLAYER_1_WON ? y2 : y1;

        const char* t1 = "VICTORY";
        const char* t2 = "DEFEAT";

        DrawRectangleGradientH(
            m_width / 4, m_height / 8, m_width / 2, m_height / 4, DARKGRAY, GRAY);
        DrawRectangleGradientH(
            m_width / 4, m_height / 8 * 5, m_width / 2, m_height / 4, DARKGRAY, GRAY);

        DrawText(t1, (m_width - MeasureText(t1, font_size)) / 2, p1, font_size, GREEN);
        DrawText(t2, (m_width - MeasureText(t2, font_size)) / 2, p2, font_size, RED);
    }
}
}
