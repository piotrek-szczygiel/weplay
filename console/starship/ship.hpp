#pragma once
#include "column.hpp"
#include "ship_camera.hpp"
#include <raylib.h>
#include <vector>

class Ship {
private:
    Vector3 speed;

    float strength;
    double last_collision_timestamp;

public:
    struct Controls {
        bool forward;
        bool back;
        bool left;
        bool right;
        bool up;
        bool down;
    };

    Vector3 position;
    Ship_Camera camera;

    Controls controls;

    Ship(Vector3 position, float strength)
        : speed {}
        , strength { strength }
        , last_collision_timestamp { -1.0 }
        , position { position }
        , camera {}
        , controls {}
    {
    }

    void update(float dt, Vector3 map_size, const std::vector<Column>& columns);
};
