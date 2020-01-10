#pragma once
#include "Column.hpp"
#include "ShipCamera.hpp"
#include <raylib.h>
#include <vector>

namespace Starship {

struct ShipControls {
    bool forward;
    bool back;
    bool up;
    bool down;

    float roll;
};

class Ship {
public:
    Ship(Vector3 position, float strength)
        : m_position { position }
        , m_camera {}
        , m_controls {}
        , m_speed {}
        , m_strength { strength }
        , m_last_collision { -1.0 }
    {
    }

    void update(float dt, Vector3 map_size, const std::vector<Column>& columns);

    void set_controls(ShipControls controls) { m_controls = controls; }

    Vector3 position() { return m_position; }

    Camera camera() { return m_camera.camera; }

    float roll() { return m_controls.roll; }

private:
    Vector3 m_position;
    ShipCamera m_camera;

    ShipControls m_controls;
    Vector3 m_speed;

    float m_strength;
    double m_last_collision;
};

}
