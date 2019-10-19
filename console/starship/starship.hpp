#pragma once
#include "../state.hpp"
#include "column.hpp"
#include "ship.hpp"
#include <algorithm>
#include <vector>

class Starship final : public State {
private:
    RenderTexture2D framebuffer;

    Ship ship;
    Vector3 map_size;

    std::vector<Column> columns;

public:
    Starship()
        : framebuffer { LoadRenderTexture(GetScreenWidth(), GetScreenHeight()) }
        , ship { { 0.0, 5.0, 5.0 }, 50.0F }
        , map_size { 40.0F, 30.0F, 10'000.0F }
        , columns { Column::generate_random_columns(static_cast<size_t>(map_size.z / 10.0F), map_size, true) }
    {
    }

    void update(std::shared_ptr<Controller::State> state) override;
    void draw() override;
    RenderTexture2D get_framebuffer() override;
};
