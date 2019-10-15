#pragma once
#include <raylib.h>
#include <vector>

class Column {
public:
    Vector3 position;
    Vector3 size;
    Color color;

    Column(Vector3 position, Vector3 size, Color color)
        : position(position)
        , size(size)
        , color(color)
    {
    }

    void draw() const;

    [[nodiscard]] bool check_collision(Vector3 ship) const;

    static std::vector<Column> generate_random_columns(size_t n, Vector3 map_size);
};
