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
        : m_framebuffer { LoadRenderTexture(GetScreenWidth(), GetScreenHeight()) }
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
    RenderTexture2D m_framebuffer;
    StateChange m_state_change { StateChange::None };

    Ship m_ship { { 0.0, 5.0, 5.0 }, 50.0F };
    Vector3 m_map_size { 40.0F, 30.0F, 10'000.0F };

    std::vector<Column> m_columns;
};

}
