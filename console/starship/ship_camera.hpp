#pragma once
#include <raylib.h>

class Ship_Camera {
public:
    Camera camera;
    Vector3 angle;

    Ship_Camera()
        : camera { {}, {}, {}, 60.0F, CAMERA_PERSPECTIVE }
        , angle { PI, 0.0F, 0.0F }
    {
    }

    void update();
};
