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
    int score;
};

const int PLAYER_SPEED = 700;
const int BALL_SPEED = 850;
const int PLAYER_WIDTH = 15;
const int PLAYER_HEIGHT = 180;
const int BALL_RADIUS = 15;
const float FRICTION = 0.8F;
const int FONT_SIZE = 24;

class Pong final : public State {

public:
    Pong()
        : m_width { 1920 }
        , m_height { 1280 }
        , m_framebuffer { LoadRenderTexture(m_width, m_height) }
        , m_player_1 { { 20, static_cast<float>(m_height / 2 - PLAYER_WIDTH / 2) }, { 0, 0 }, 0 }
        , m_player_2 { { static_cast<float>(m_width - 20 - PLAYER_WIDTH),
                           static_cast<float>(m_height / 2 - PLAYER_WIDTH / 2) },
            { 0, 0 }, 0 }
        , m_ball { { static_cast<float>(m_width / 2), static_cast<float>(m_height / 2) },
            { 0.9F, 0.17F } } // normalized (1, 1) vector

    {
    }
    void update(std::shared_ptr<ControllerState> state) override;
    void draw() override;
    RenderTexture2D framebuffer() override;
    void restart();

private:
    int m_width;
    int m_height;
    RenderTexture2D m_framebuffer;

    Player m_player_1;
    Player m_player_2;
    Ball m_ball;
};
}