#include "Starship.hpp"
#include "Util.hpp"
#include <algorithm>
#include <cmath>

void Starship::Initialize() { framebuffer = LoadRenderTexture(GetScreenWidth(), GetScreenHeight()); }

void Starship::Update()
{
    float dt = GetFrameTime();

    ship.forward = IsKeyDown(KEY_W);
    ship.back = IsKeyDown(KEY_S);
    ship.left = IsKeyDown(KEY_A);
    ship.right = IsKeyDown(KEY_D);
    ship.up = IsKeyDown(KEY_UP);
    ship.down = IsKeyDown(KEY_DOWN);

    ship.Update(dt, mapSize, columns);
}

void Starship::Draw()
{
    BeginTextureMode(framebuffer);
    ClearBackground(BLACK);
    BeginMode3D(ship.camera.camera);

    DrawPlane(Vector3 { 0.0F, 0.0F, mapSize.z / 2.0F }, Vector2 { mapSize.x, mapSize.z }, DARKBROWN);

    size_t columnsDrawn = 0;
    bool firstColumnDrawn = false;

    const float renderDistance = 300.0F;

    for (auto& column : columns) {
        float distance = column.position.z - ship.position.z;

        if (distance < 0.0F) {
            continue;
        }

        if (distance < renderDistance) {

            if (distance > renderDistance * 0.5F) {
                column.color.a
                    = static_cast<unsigned char>((1.0F - ((distance / renderDistance) - 0.5F) * 2.0F) * 255.0F);
            } else {
                column.color.a = 255;
            }

            column.Draw();

            ++columnsDrawn;
            firstColumnDrawn = true;

            if (columnsDrawn % 100 == 0) {
                EndMode3D();
                BeginMode3D(ship.camera.camera);
            }
        } else if (firstColumnDrawn) {
            break;
        }
    }

    EndMode3D();
    EndTextureMode();
}

RenderTexture2D Starship::GetFramebuffer() { return framebuffer; }

void Ship::Update(float dt, Vector3 mapSize, const std::vector<Column>& columns)
{
    double now = GetTime();
    if (now - collisionStart < 0.75) {
        speed.x = Fade(speed.x, 0.0F, dt * strength / 4.0F);
        speed.y = Fade(speed.y, 0.0F, dt * strength / 4.0F);
        speed.z = Fade(speed.z, 0.0F, dt * strength / 4.0F);
    } else {

        if (forward && !back) {
            speed.z += dt * strength * 2.0F;
        } else if (back && !forward) {
            if (speed.z > 0.0F) {
                speed.z -= dt * strength * 2.0F;
            } else {
                speed.z -= dt * strength;
            }
        } else {
            speed.z = Fade(speed.z, 0.0F, dt * strength / 2.0F);
        }

        if (left && !right) {
            if (speed.x > 0.0F) {
                speed.x += dt * strength * 4.0F;
            } else {
                speed.x += dt * strength * 8.0F;
            }
        } else if (right && !left) {
            if (speed.x < 0.0F) {
                speed.x -= dt * strength * 4.0F;
            } else {
                speed.x -= dt * strength * 8.0F;
            }
        } else {
            speed.x = Fade(speed.x, 0, dt * strength * 2.0F);
        }

        if (up && !down) {
            speed.y += dt * strength * 2.0F;
        } else if (down && !up) {
            speed.y -= dt * strength * 2.0F;
        } else {
            speed.y = Fade(speed.y, 0, dt * strength * 4.0F);
        }
    }

    speed = Vector3 {
        std::clamp(speed.x, -strength / 2.0F, strength / 2.0F),
        std::clamp(speed.y, -strength, strength),
        std::clamp(speed.z, -strength / 4.0F, strength * 2.0F),
    };

    camera.angle.z = speed.x / strength * -PI / 8.0F;

    Vector3 offset {
        speed.x * dt,
        speed.y * dt,
        speed.z * dt,
    };

    Vector3 newPosition {
        std::clamp(position.x + offset.x, -mapSize.x / 2.0F + 1.0F, mapSize.x / 2.0F - 1.0F),
        std::clamp(position.y + offset.y, 1.0F, mapSize.y),
        std::clamp(position.z + offset.z, 0.0F, mapSize.z),
    };

    bool colliding = false;

    for (const auto& column : columns) {
        if (column.position.z < newPosition.z) {
            continue;
        }

        if (column.Collision(newPosition)) {
            colliding = true;
            break;
        }

        if (column.position.z > newPosition.z + 10.0F) {
            break;
        }
    }

    if (colliding) {
        if (speed.z > strength) {
            collisionStart = now;
        }

        speed.x = -speed.x / 10.0F;
        speed.y = -speed.y / 10.0F;
        speed.z = -speed.z / 10.0F;
    } else {
        position = newPosition;
    }

    camera.camera.position = position;
    camera.Update();
}

bool Column::Collision(Vector3 ship) const
{
    return Between(ship.x, position.x, size.x / 2.0F + 1.0F) && Between(ship.y, position.y, size.y / 2.0F + 1.0F)
        && Between(ship.z, position.z, size.z / 2.0F + 1.0F);
}

void ShipCamera::Update()
{
    const float focusDistance = 25.0F;

    angle.y = std::clamp(angle.y, -1.48F, 1.48F);
    if (angle.x > 2.0F * PI) {
        angle.x -= 2.0F * PI;
    } else if (angle.x < -2.0F * PI) {
        angle.x += 2.0F * PI;
    }

    camera.target = Vector3 {
        camera.position.x - sinf(angle.x) * focusDistance,
        camera.position.y + sinf(angle.y) * focusDistance,
        camera.position.z - cosf(angle.x) * focusDistance,
    };

    Vector3 direction {
        (camera.target.x - camera.position.x) / focusDistance,
        (camera.target.y - camera.position.y) / focusDistance,
        (camera.target.z - camera.position.z) / focusDistance,
    };

    float alpha = angle.z * -PI / 8;

    camera.up = Vector3 {
        alpha * direction.z - direction.x * direction.y,
        direction.x * direction.x + direction.z * direction.z,
        -direction.y * direction.z - alpha * direction.x,
    };
}

void Column::Draw() const { DrawCube(position, size.x, size.y, size.z, color); }

std::vector<Column> Column::GenerateColumns(size_t n, Vector3 mapSize)
{
    std::vector<Column> columns;

    for (size_t i = 0; i < n; ++i) {
        float height = Random(mapSize.y / 4.0F, mapSize.y * 2.0F);

        Vector3 position;
        Vector3 size;

        Color color {
            static_cast<unsigned char>(Random(20, 255)),
            static_cast<unsigned char>(Random(10, 55)),
            static_cast<unsigned char>(Random(30, 60)),
            255,
        };

        if (i % 10 == 0) {
            position = {
                0.0F,
                std::roundf(Random(1.0F, height)),
                std::roundf(Random(50.0F, mapSize.z)),
            };

            size = {
                mapSize.x,
                2.0F,
                2.0F,
            };
        } else {
            position = {
                std::roundf(Random(-mapSize.x / 2 + 1.0F, mapSize.x / 2 - 1.0F)),
                height / 2,
                std::roundf(Random(50.0F, mapSize.z)),
            };

            size = {
                2.0F,
                height,
                2.0F,
            };
        }

        columns.emplace_back(position, size, color);
    }

    return columns;
}
