#include "ShipCamera.hpp"
#include "../Util.hpp"
#include <cmath>

void ShipCamera::update()
{
    static const float focusDistance { 25.0F };

    angle.y = clamp(angle.y, -1.48F, 1.48F);
    if (angle.x > 2.0F * PI) {
        angle.x -= 2.0F * PI;
    } else if (angle.x < -2.0F * PI) {
        angle.x += 2.0F * PI;
    }

    camera.target = {
        camera.position.x - std::sin(angle.x) * focusDistance,
        camera.position.y + std::sin(angle.y) * focusDistance,
        camera.position.z - std::cos(angle.x) * focusDistance,
    };

    Vector3 direction {
        (camera.target.x - camera.position.x) / focusDistance,
        (camera.target.y - camera.position.y) / focusDistance,
        (camera.target.z - camera.position.z) / focusDistance,
    };

    float alpha { angle.z * -PI / 8 };

    camera.up = {
        alpha * direction.z - direction.x * direction.y,
        direction.x * direction.x + direction.z * direction.z,
        -direction.y * direction.z - alpha * direction.x,
    };
}
