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
    std::vector<Column> columns;

    Vector3 map_size;

public:
    Starship()
        : framebuffer({})
        , ship(Ship(Vector3{ 0.0, 5.0, 5.0 }, 50.0F))
        , map_size(Vector3{ 40.0F, 30.0F, 10'000.0F })
    {
        columns = Column::generate_random_columns(static_cast<size_t>(map_size.z / 10.0F), map_size);
        std::sort(columns.begin(), columns.end(),
            [](const Column& a, const Column& b) -> bool { return a.position.z < b.position.z; });
    }

    Starship(const Starship&) = delete;
    Starship& operator=(const Starship&) = delete;

    Starship(Starship&&) = delete;
    Starship& operator=(Starship&&) = delete;

    ~Starship() override = default;

    void init() override;
    void update() override;
    void draw() override;
    RenderTexture2D get_framebuffer() override;
};