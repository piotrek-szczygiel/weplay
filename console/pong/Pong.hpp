#include "../State.hpp"
#include "raylib.h"
#include <boost/math/special_functions/sign.hpp>
#include <charconv>
#include <cmath>
#include <cstdio>
#include <random>

namespace Pong {

struct Player {
    Vector2 position;
    Vector2 speed;
    int score;
};

struct Ball {
    Vector2 position;
    Vector2 speed;
};

const float PLAYER_SPEED = 700.0F;
const float BALL_SPEED = 850.0F;
const float PLAYER_WIDTH = 15.0F;
const float PLAYER_HEIGHT = 180.0F;
const float BALL_RADIUS = 15.0F;
const float FRICTION = 0.8F;
const int FONT_SIZE = 24;

class Pong final : public State {

public:
    Pong()
        : m_width { 1024 }
        , m_height { 768 }
        , m_framebuffer { LoadRenderTexture(static_cast<int>(m_width), static_cast<int>(m_height)) }
    {
        restart();
    }

    void update(std::shared_ptr<ControllerState> state) override;
    void draw() override;
    RenderTexture2D framebuffer() override;
    void restart();

private:
    float m_width;
    float m_height;
    RenderTexture2D m_framebuffer;

    Player m_player_1 {};
    Player m_player_2 {};
    Ball m_ball {};

    std::string m_score {};
    int m_score_position {};

    std::mt19937 m_gen { std::random_device {}() };
};
}