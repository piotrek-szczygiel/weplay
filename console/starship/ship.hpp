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
    Vector3 position;
    Ship_Camera camera;
    bool forward, back, left, right, up, down;

    Ship(Vector3 position, float strength)
        : position(position)
        , speed(Vector3 {})
        , strength(strength)
        , last_collision_timestamp(0.0)
    {
        forward = back = left = right = up = down = false;
    }
    void update(float dt, Vector3 map_size, const std::vector<Column>& columns);
};