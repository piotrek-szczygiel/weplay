#pragma once
#include "State.h"
#include <vector>
struct Column {
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

    static std::vector<Column> GenerateColumns(size_t n, Vector3 mapSize);
};

struct ShipCamera {
    Camera camera;
    Vector3 angle;

    ShipCamera();
    void Update();
};

struct Ship {
    Vector3 position;
    Vector3 speed;
    ShipCamera camera;

    float strength;

    bool forward, back, left, right, up, down;
    bool collision;

    Ship(Vector3 position, float strength)
        : position(position)
        , strength(strength)
    {
        forward = back = left = right = up = down = collision = false;
        speed = Vector3 {};
    }
    void Update(float dt);
};

class Starship : public State {
private:
    RenderTexture2D framebuffer;

    Ship ship;
    std::vector<Column> columns;

    Vector3 mapSize;

public:
    Starship()
        : ship(Ship(Vector3 { 0.0, 5.0, 5.0 }, 50.0F))
        , framebuffer({})
        , mapSize(Vector3 { 40.0F, 30.0F, 10'000.0F })
    {
        columns = Column::GenerateColumns(static_cast<size_t>(mapSize.z / 10.0F), mapSize);
    }

    void Initialize() override;
    void Update() override;
    void Draw() override;
    RenderTexture2D GetFramebuffer() override;
};