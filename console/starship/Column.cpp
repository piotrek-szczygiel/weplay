#include "Column.hpp"
#include "../Util.hpp"
#include <algorithm>
#include <cmath>
#include <random.hpp>

using Random = effolkronium::random_static;

namespace Starship {

void Column::draw() const { DrawCube(m_position, m_size.x, m_size.y, m_size.z, m_color); }

bool Column::collision(Vector3 ship) const
{
    return threshold(ship.x, m_position.x, m_size.x / 2.0F + 0.5F)
        && threshold(ship.y, m_position.y, m_size.y / 2.0F + 0.5F)
        && threshold(ship.z, m_position.z, m_size.z / 2.0F + 0.5F);
}

std::vector<Column> Column::generate_random_columns(size_t n, Vector3 map_size, bool horizontal)
{
    std::vector<Column> columns {};

    for (size_t i = 0; i < n; ++i) {
        float height { Random::get(map_size.y / 4.0F, map_size.y * 2.0F) };

        Color color {
            static_cast<unsigned char>(Random::get(20, 255)),
            static_cast<unsigned char>(Random::get(10, 55)),
            static_cast<unsigned char>(Random::get(30, 60)),
            255,
        };

        if (horizontal && i % 20 == 0) {
            color.a = 200;

            Vector3 position {
                0.0F,
                std::roundf(Random::get(1.0F, map_size.y)),
                std::roundf(Random::get(50.0F, map_size.z)),
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
                std::roundf(Random::get(-map_size.x / 2.0F + 1.0F, map_size.x / 2.0F - 1.0F)),
                height / 2.0F,
                std::roundf(Random::get(50.0F, map_size.z)),
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
        [](const Column& a, const Column& b) -> bool { return a.m_position.z < b.m_position.z; });

    return columns;
}
}
