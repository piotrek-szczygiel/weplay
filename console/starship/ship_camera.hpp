#pragma once
#include <raylib.h>

class Ship_Camera {
public:
    Camera camera;
    Vector3 angle;

    Ship_Camera()
        : camera(Camera {})
        , angle(Vector3 { PI, 0.0F, 0.0F })
    {
        camera.fovy = 60.0F;
        camera.type = CAMERA_PERSPECTIVE;
    }

    void update();
};
