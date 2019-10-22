#pragma once
#include <raylib.h>
#include <vector>

namespace Starship {

class Column {
public:
    Column(Vector3 position, Vector3 size, Color color)
        : m_position { position }
        , m_size { size }
        , m_color { color }
    {
    }

    void draw() const;
    bool collision(Vector3 ship) const;

    Vector3 position() const { return m_position; }

    void set_alpha(int alpha) { m_color.a = alpha; }

    static std::vector<Column> generate_random_columns(size_t n, Vector3 map_size, bool horizontal);

private:
    Vector3 m_position;
    Vector3 m_size;
    Color m_color;
};

}
