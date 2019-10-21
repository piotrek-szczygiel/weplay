#include "Ship.hpp"
#include "../Util.hpp"

namespace Starship {

void Ship::update(float dt, Vector3 mapSize, const std::vector<Column>& columns)
{
    double now { GetTime() };

    if (now - lastCollision_ < 0.75) {
        speed_ = {
            fade(speed_.x, 0.0F, dt * strength_ / 4.0F),
            speed_.y = fade(speed_.y, 0.0F, dt * strength_ / 4.0F),
            speed_.z = fade(speed_.z, 0.0F, dt * strength_ / 4.0F),
        };
    } else {

        if (controls.forward && !controls.back) {
            speed_.z += dt * strength_ * 2.0F;
        } else if (controls.back && !controls.forward) {
            if (speed_.z > 0.0F) {
                speed_.z -= dt * strength_ * 2.0F;
            } else {
                speed_.z -= dt * strength_;
            }
        } else {
            speed_.z = fade(speed_.z, 0.0F, dt * strength_ / 2.0F);
        }

        if (controls.left && !controls.right) {
            if (speed_.x > 0.0F) {
                speed_.x += dt * strength_ * 4.0F;
            } else {
                speed_.x += dt * strength_ * 8.0F;
            }
        } else if (controls.right && !controls.left) {
            if (speed_.x < 0.0F) {
                speed_.x -= dt * strength_ * 4.0F;
            } else {
                speed_.x -= dt * strength_ * 8.0F;
            }
        } else {
            speed_.x = fade(speed_.x, 0, dt * strength_ * 2.0F);
        }

        if (controls.up && !controls.down) {
            speed_.y += dt * strength_ * 2.0F;
        } else if (controls.down && !controls.up) {
            speed_.y -= dt * strength_ * 2.0F;
        } else {
            speed_.y = fade(speed_.y, 0, dt * strength_ * 4.0F);
        }
    }

    speed_ = {
        clamp(speed_.x, -strength_ / 2.0F, strength_ / 2.0F),
        clamp(speed_.y, -strength_, strength_),
        clamp(speed_.z, -strength_ / 4.0F, strength_ * 2.0F),
    };

    camera.angle.z = speed_.x / strength_ * -PI / 8.0F;

    Vector3 offset {
        speed_.x * dt,
        speed_.y * dt,
        speed_.z * dt,
    };

    Vector3 newPosition {
        clamp(position.x + offset.x, -mapSize.x / 2.0F + 1.0F, mapSize.x / 2.0F - 1.0F),
        clamp(position.y + offset.y, 1.0F, mapSize.y),
        clamp(position.z + offset.z, 0.0F, mapSize.z),
    };

    bool colliding { false };

    for (const auto& column : columns) {
        if (column.position.z < newPosition.z) {
            continue;
        }

        if (column.checkCollision(newPosition)) {
            colliding = true;
            break;
        }

        if (column.position.z > newPosition.z + 10.0F) {
            break;
        }
    }

    if (colliding) {
        if (speed_.z > strength_) {
            lastCollision_ = now;
        }

        speed_ = {
            -speed_.x / 10.0F,
            -speed_.y / 10.0F,
            -speed_.z / 10.0F,
        };
    } else {
        position = newPosition;
    }

    camera.camera.position = position;
    camera.update();
}

}
