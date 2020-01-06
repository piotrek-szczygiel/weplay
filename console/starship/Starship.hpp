#pragma once
#include "../State.hpp"
#include "Column.hpp"
#include "Ship.hpp"
#include <algorithm>
#include <vector>

namespace Starship {

class Starship final : public State {
public:
    Starship()
        : m_width { GetScreenWidth() }
        , m_height { GetScreenHeight() }
        , m_framebuffer { LoadRenderTexture(m_width, m_height) }
        , m_view_1 { LoadRenderTexture(m_width, m_height / 2) }
        , m_view_2 { LoadRenderTexture(m_width, m_height / 2) }
        , m_view_source { 0.0F, 0.0F, static_cast<float>(m_width),
            -static_cast<float>(m_height) / 2.0F }
        , m_view_dest_1 { 0.0F, 0.0F, static_cast<float>(m_width),
            static_cast<float>(m_height) / 2.0F }
        , m_view_dest_2 { 0.0F, static_cast<float>(m_height) / 2.0F, static_cast<float>(m_width),
            static_cast<float>(m_height) / 2.0F }
        , m_columns { Column::generate_random_columns(
              static_cast<size_t>(m_map_size.z / 10.0F), m_map_size, true) }
    {
    }

    ~Starship() override { UnloadRenderTexture(m_framebuffer); }

    void update(const std::vector<ControllerState>& controllers) override;
    void draw() override;
    RenderTexture2D framebuffer() override;
    StateChange state_change() override { return m_state_change; }

private:
    int m_width;
    int m_height;

    RenderTexture2D m_framebuffer;
    RenderTexture2D m_view_1;
    RenderTexture2D m_view_2;

    Rectangle m_view_source;
    Rectangle m_view_dest_1;
    Rectangle m_view_dest_2;

    StateChange m_state_change { StateChange::None };

    Ship m_player_1 { { 0.0, 5.0, 5.0 }, 50.0F };
    Ship m_player_2 { { 0.0, 5.0, 5.0 }, 50.0F };

    Vector3 m_map_size { 40.0F, 30.0F, 10'000.0F };

    std::vector<Column> m_columns;
};
}
