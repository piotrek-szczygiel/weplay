#pragma once
#include <raylib.h>

namespace Starship {

class ShipCamera {
public:
    Camera camera;
    Vector3 angle;

    ShipCamera()
        : camera { {}, {}, {}, 60.0F, CAMERA_PERSPECTIVE }
        , angle { PI, 0.0F, 0.0F }
    {
    }

    void update();
};

}
