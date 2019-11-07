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
const int ANIM_FONT_SIZE = 100;

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
    StateChange state_change() override { return m_state_change; }

private:
    enum State { PLAYING, SCORING };
    enum AnimationState { SHADOWING, LIGHTING };

    float m_width;
    float m_height;
    RenderTexture2D m_framebuffer;
    StateChange m_state_change { StateChange::None };

    State m_state { PLAYING };
    AnimationState m_anim_state { SHADOWING };
    float m_animation_timer { 0.0F };

    Player m_player_1 {};
    Player m_player_2 {};
    Ball m_ball {};

    std::string m_score {};
    std::string m_last_score {};
    int m_score_position {};

    std::mt19937 m_gen { std::random_device {}() };

    void restart();
    void add_score(int id);
    void change_state(State state);
    int text_position_center(std::string& string, int font_size);
};

}
