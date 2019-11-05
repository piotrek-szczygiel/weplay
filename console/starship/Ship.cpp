#include "Ship.hpp"
#include "../Util.hpp"

namespace Starship {

void Ship::update(float dt, Vector3 map_size, const std::vector<Column>& columns)
{
    double now { GetTime() };

    if (now - m_last_collision < 0.75) {
        m_speed = {
            fade(m_speed.x, 0.0F, dt * m_strength / 4.0F),
            m_speed.y = fade(m_speed.y, 0.0F, dt * m_strength / 4.0F),
            m_speed.z = fade(m_speed.z, 0.0F, dt * m_strength / 4.0F),
        };
    } else {
        if (m_controls.forward && !m_controls.back) {
            m_speed.z += dt * m_strength * 2.0F;
        } else if (m_controls.back && !m_controls.forward) {
            if (m_speed.z > 0.0F) {
                m_speed.z -= dt * m_strength * 2.0F;
            } else {
                m_speed.z -= dt * m_strength;
            }
        } else {
            m_speed.z = fade(m_speed.z, 0.0F, dt * m_strength / 2.0F);
        }

        if (m_controls.up && !m_controls.down) {
            m_speed.y += dt * m_strength * 2.0F;
        } else if (m_controls.down && !m_controls.up) {
            m_speed.y -= dt * m_strength * 2.0F;
        } else {
            m_speed.y = fade(m_speed.y, 0, dt * m_strength * 4.0F);
        }
    }

    m_speed.x = -m_controls.roll * m_strength;

    m_speed = {
        clamp(m_speed.x, -m_strength, m_strength),
        clamp(m_speed.y, -m_strength, m_strength),
        clamp(m_speed.z, -m_strength / 4.0F, m_strength * 2.0F),
    };

    m_camera.angle.z = m_speed.x / m_strength * -PI / 8.0F;

    Vector3 offset {
        m_speed.x * dt,
        m_speed.y * dt,
        m_speed.z * dt,
    };

    Vector3 new_position {
        clamp(m_position.x + offset.x, -map_size.x / 2.0F + 1.0F, map_size.x / 2.0F - 1.0F),
        clamp(m_position.y + offset.y, 1.0F, map_size.y),
        clamp(m_position.z + offset.z, 0.0F, map_size.z),
    };

    bool colliding { false };

    for (const auto& column : columns) {
        if (column.position().z < new_position.z) {
            continue;
        }

        if (column.collision(new_position)) {
            colliding = true;
            break;
        }

        if (column.position().z > new_position.z + 10.0F) {
            break;
        }
    }

    if (colliding) {
        if (m_speed.z > m_strength) {
            m_last_collision = now;
        }

        m_speed = {
            -m_speed.x / 10.0F,
            -m_speed.y / 10.0F,
            -m_speed.z / 10.0F,
        };
    } else {
        m_position = new_position;
    }

    m_camera.camera.position = m_position;
    m_camera.update();
}

}
