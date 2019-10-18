#include "ship.hpp"
#include "../util.hpp"

void Ship::update(float dt, Vector3 map_size, const std::vector<Column>& columns)
{
    double now { GetTime() };

    if (now - last_collision_timestamp < 0.75) {
        speed = {
            fade(speed.x, 0.0F, dt * strength / 4.0F),
            speed.y = fade(speed.y, 0.0F, dt * strength / 4.0F),
            speed.z = fade(speed.z, 0.0F, dt * strength / 4.0F),
        };
    } else {

        if (controls.forward && !controls.back) {
            speed.z += dt * strength * 2.0F;
        } else if (controls.back && !controls.forward) {
            if (speed.z > 0.0F) {
                speed.z -= dt * strength * 2.0F;
            } else {
                speed.z -= dt * strength;
            }
        } else {
            speed.z = fade(speed.z, 0.0F, dt * strength / 2.0F);
        }

        if (controls.left && !controls.right) {
            if (speed.x > 0.0F) {
                speed.x += dt * strength * 4.0F;
            } else {
                speed.x += dt * strength * 8.0F;
            }
        } else if (controls.right && !controls.left) {
            if (speed.x < 0.0F) {
                speed.x -= dt * strength * 4.0F;
            } else {
                speed.x -= dt * strength * 8.0F;
            }
        } else {
            speed.x = fade(speed.x, 0, dt * strength * 2.0F);
        }

        if (controls.up && !controls.down) {
            speed.y += dt * strength * 2.0F;
        } else if (controls.down && !controls.up) {
            speed.y -= dt * strength * 2.0F;
        } else {
            speed.y = fade(speed.y, 0, dt * strength * 4.0F);
        }
    }

    speed = {
        clamp(speed.x, -strength / 2.0F, strength / 2.0F),
        clamp(speed.y, -strength, strength),
        clamp(speed.z, -strength / 4.0F, strength * 2.0F),
    };

    camera.angle.z = speed.x / strength * -PI / 8.0F;

    Vector3 offset {
        speed.x * dt,
        speed.y * dt,
        speed.z * dt,
    };

    Vector3 new_position {
        clamp(position.x + offset.x, -map_size.x / 2.0F + 1.0F, map_size.x / 2.0F - 1.0F),
        clamp(position.y + offset.y, 1.0F, map_size.y),
        clamp(position.z + offset.z, 0.0F, map_size.z),
    };

    bool colliding { false };

    for (const auto& column : columns) {
        if (column.position.z < new_position.z) {
            continue;
        }

        if (column.check_collision(new_position)) {
            colliding = true;
            break;
        }

        if (column.position.z > new_position.z + 10.0F) {
            break;
        }
    }

    if (colliding) {
        if (speed.z > strength) {
            last_collision_timestamp = now;
        }

        speed = {
            -speed.x / 10.0F,
            -speed.y / 10.0F,
            -speed.z / 10.0F,
        };
    } else {
        position = new_position;
    }

    camera.camera.position = position;
    camera.update();
}
