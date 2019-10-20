#pragma once
#include "../State.hpp"
#include "Column.hpp"
#include "Ship.hpp"
#include <algorithm>
#include <vector>

class Starship final : public State {
public:
    Starship()
        : framebuffer_ { LoadRenderTexture(GetScreenWidth(), GetScreenHeight()) }
        , ship_ { { 0.0, 5.0, 5.0 }, 50.0F }
        , mapSize_ { 40.0F, 30.0F, 10'000.0F }
        , columns_ { Column::generateRandomColumns(static_cast<size_t>(mapSize_.z / 10.0F), mapSize_, true) }
    {
    }

    ~Starship() override
    {
        UnloadRenderTexture(framebuffer_);
    }

    void update(std::shared_ptr<Controller::State> state) override;
    void draw() override;
    RenderTexture2D getFramebuffer() override;

private:
    RenderTexture2D framebuffer_;

    Ship ship_;
    Vector3 mapSize_;

    std::vector<Column> columns_;
};
