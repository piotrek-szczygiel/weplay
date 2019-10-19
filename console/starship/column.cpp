#include "column.hpp"
#include "../util.hpp"
#include <algorithm>
#include <cmath>

void Column::draw() const
{
    DrawCube(position, size.x, size.y, size.z, color);
}

bool Column::check_collision(Vector3 ship) const
{
    return threshold(ship.x, position.x, size.x / 2.0F + 0.5F) && threshold(ship.y, position.y, size.y / 2.0F + 0.5F)
        && threshold(ship.z, position.z, size.z / 2.0F + 0.5F);
}

std::vector<Column> Column::generate_random_columns(size_t n, Vector3 map_size, bool horizontal)
{
    std::vector<Column> columns {};

    for (size_t i = 0; i < n; ++i) {
        float height { random(map_size.y / 4.0F, map_size.y * 2.0F) };

        Color color {
            static_cast<unsigned char>(random(20, 255)),
            static_cast<unsigned char>(random(10, 55)),
            static_cast<unsigned char>(random(30, 60)),
            255,
        };

        if (horizontal && i % 20 == 0) {
            color.a = 200;

            Vector3 position {
                0.0F,
                std::roundf(random(1.0F, map_size.y)),
                std::roundf(random(50.0F, map_size.z)),
            };

            Vector3 size {
                map_size.x,
                2.0F,
                2.0F,
            };

            columns.emplace_back(position, size, color);

            Vector3 pole_position {
                -map_size.x / 2.0F + 1.0F,
                position.y / 2.0F,
                position.z,
            };
            Vector3 pole_size {
                2.0F,
                position.y,
                2.0F,
            };

            columns.emplace_back(pole_position, pole_size, color);
            pole_position.x *= -1.0F;
            columns.emplace_back(pole_position, pole_size, color);
        } else {
            Vector3 position {
                std::roundf(random(-map_size.x / 2.0F + 1.0F, map_size.x / 2.0F - 1.0F)),
                height / 2.0F,
                std::roundf(random(50.0F, map_size.z)),
            };

            Vector3 size {
                2.0F,
                height,
                2.0F,
            };

            columns.emplace_back(position, size, color);
        }
    }

    std::sort(columns.begin(), columns.end(),
        [](const Column& a, const Column& b) -> bool { return a.position.z < b.position.z; });

    return columns;
}
