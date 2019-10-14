#pragma once
#include "State.hpp"
#include <vector>

class Column {
private:
    Vector3 position;
    Vector3 size;
    Color color;

public:
    Column(Vector3 position, Vector3 size, Color color)
        : position(position)
        , size(size)
        , color(color)
    {
    }

    void Draw() const;

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
    Vector3 position;
    Vector3 speed;

    float strength;
    bool collision;

public:
    ShipCamera camera;
    bool forward, back, left, right, up, down;

    Ship(Vector3 position, float strength)
        : position(position)
        , speed(Vector3 {})
        , strength(strength)
    {
        forward = back = left = right = up = down = collision = false;
    }
    void Update(float dt);
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