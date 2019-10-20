#pragma once
#include "Column.hpp"
#include "ShipCamera.hpp"
#include <raylib.h>
#include <vector>

class Ship {
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
    ShipCamera camera;

    Controls controls;

    Ship(Vector3 position, float strength)
        : speed_ {}
        , strength_ { strength }
        , lastCollision_ { -1.0 }
        , position { position }
        , camera {}
        , controls {}
    {
    }

    void update(float dt, Vector3 mapSize, const std::vector<Column>& columns);

private:
    Vector3 speed_;

    float strength_;
    double lastCollision_;
};
