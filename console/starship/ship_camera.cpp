#include "ship_camera.hpp"
#include <algorithm>
#include <cmath>

void Ship_Camera::update()
{
    const float focus_distance = 25.0F;

    angle.y = std::clamp(angle.y, -1.48F, 1.48F);
    if (angle.x > 2.0F * PI) {
        angle.x -= 2.0F * PI;
    } else if (angle.x < -2.0F * PI) {
        angle.x += 2.0F * PI;
    }

    camera.target = Vector3 {
        camera.position.x - std::sinf(angle.x) * focus_distance,
        camera.position.y + std::sinf(angle.y) * focus_distance,
        camera.position.z - std::cosf(angle.x) * focus_distance,
    };

    Vector3 direction {
        (camera.target.x - camera.position.x) / focus_distance,
        (camera.target.y - camera.position.y) / focus_distance,
        (camera.target.z - camera.position.z) / focus_distance,
    };

    float alpha = angle.z * -PI / 8;

    camera.up = Vector3 {
        alpha * direction.z - direction.x * direction.y,
        direction.x * direction.x + direction.z * direction.z,
        -direction.y * direction.z - alpha * direction.x,
    };
}