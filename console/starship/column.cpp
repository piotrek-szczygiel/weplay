#include "column.hpp"
#include "../util.hpp"
#include <cmath>

void Column::draw() const { DrawCube(position, size.x, size.y, size.z, color); }

bool Column::check_collision(Vector3 ship) const
{
    return threshold(ship.x, position.x, size.x / 2.0F + 1.0F) && threshold(ship.y, position.y, size.y / 2.0F + 1.0F)
        && threshold(ship.z, position.z, size.z / 2.0F + 1.0F);
}

std::vector<Column> Column::generate_random_columns(size_t n, Vector3 map_size)
{
    std::vector<Column> columns;

    for (size_t i = 0; i < n; ++i) {
        float height = random(map_size.y / 4.0F, map_size.y * 2.0F);

        Vector3 position;
        Vector3 size;

        Color color {
            static_cast<unsigned char>(random(20, 255)),
            static_cast<unsigned char>(random(10, 55)),
            static_cast<unsigned char>(random(30, 60)),
            255,
        };

        if (i % 10 == 0) {
            position = {
                0.0F,
                std::roundf(random(1.0F, height)),
                std::roundf(random(50.0F, map_size.z)),
            };

            size = {
                map_size.x,
                2.0F,
                2.0F,
            };
        } else {
            position = {
                std::roundf(random(-map_size.x / 2 + 1.0F, map_size.x / 2 - 1.0F)),
                height / 2,
                std::roundf(random(50.0F, map_size.z)),
            };

            size = {
                2.0F,
                height,
                2.0F,
            };
        }

        columns.emplace_back(position, size, color);
    }

    return columns;
}