#include "ShipCamera.hpp"
#include "../Util.hpp"
#include <cmath>

namespace Starship {

void ShipCamera::update()
{
    constexpr float FOCUS_DISTANCE { 25.0F };

    angle.y = clamp(angle.y, -1.48F, 1.48F);
    if (angle.x > 2.0F * PI) {
        angle.x -= 2.0F * PI;
    } else if (angle.x < -2.0F * PI) {
        angle.x += 2.0F * PI;
    }

    camera.target = {
        camera.position.x - std::sin(angle.x) * FOCUS_DISTANCE,
        camera.position.y + std::sin(angle.y) * FOCUS_DISTANCE,
        camera.position.z - std::cos(angle.x) * FOCUS_DISTANCE,
    };

    Vector3 direction {
        (camera.target.x - camera.position.x) / FOCUS_DISTANCE,
        (camera.target.y - camera.position.y) / FOCUS_DISTANCE,
        (camera.target.z - camera.position.z) / FOCUS_DISTANCE,
    };

    float alpha { angle.z * -PI / 8 };

    camera.up = {
        alpha * direction.z - direction.x * direction.y,
        direction.x * direction.x + direction.z * direction.z,
        -direction.y * direction.z - alpha * direction.x,
    };
}

}
