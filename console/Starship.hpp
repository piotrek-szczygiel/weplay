#pragma once
#include "State.hpp"
#include <algorithm>
#include <vector>

class Column {
public:
    Vector3 position;
    Vector3 size;
    Color color;

    Column(Vector3 position, Vector3 size, Color color)
        : position(position)
        , size(size)
        , color(color)
    {
    }

    void Draw() const;

    [[nodiscard]] bool Collision(Vector3 ship) const;

    static std::vector<Column> GenerateColumns(size_t n, Vector3 mapSize);
};

class ShipCamera {
public:
    Camera camera;
    Vector3 angle;

    ShipCamera()
        : camera(Camera {})
        , angle(Vector3 { PI, 0.0F, 0.0F })
    {
        camera.fovy = 60.0F;
        camera.type = CAMERA_PERSPECTIVE;
    }

    void Update();
};

class Ship {
private:
    Vector3 speed;

    float strength;
    double collisionStart;

public:
    Vector3 position;
    ShipCamera camera;
    bool forward, back, left, right, up, down;

    Ship(Vector3 position, float strength)
        : position(position)
        , speed(Vector3 {})
        , strength(strength)
        , collisionStart(0.0)
    {
        forward = back = left = right = up = down = false;
    }
    void Update(float dt, Vector3 mapSize, const std::vector<Column>& columns);
};

class Starship final : public State {
private:
    RenderTexture2D framebuffer;

    Ship ship;
    std::vector<Column> columns;

    Vector3 mapSize;

public:
    Starship()
        : framebuffer({})
        , ship(Ship(Vector3 { 0.0, 5.0, 5.0 }, 50.0F))
        , mapSize(Vector3 { 40.0F, 30.0F, 10'000.0F })
    {
        columns = Column::GenerateColumns(static_cast<size_t>(mapSize.z / 10.0F), mapSize);
        std::sort(columns.begin(), columns.end(),
            [](const Column& a, const Column& b) -> bool { return a.position.z < b.position.z; });
    }

    Starship(const Starship&) = delete;
    Starship& operator=(const Starship&) = delete;

    Starship(Starship&&) = delete;
    Starship& operator=(Starship&&) = delete;

    ~Starship() override = default;

    void Initialize() override;
    void Update() override;
    void Draw() override;
    RenderTexture2D GetFramebuffer() override;
};