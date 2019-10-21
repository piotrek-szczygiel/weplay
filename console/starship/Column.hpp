#pragma once
#include <raylib.h>
#include <vector>

namespace Starship {

class Column {
public:
    Vector3 position;
    Vector3 size;
    Color color;

    Column(Vector3 position, Vector3 size, Color color)
        : position { position }
        , size { size }
        , color { color }
    {
    }

    void draw() const;

    bool checkCollision(Vector3 ship) const;

    static std::vector<Column> generateRandomColumns(size_t n, Vector3 map_size, bool horizontal);
};

}
