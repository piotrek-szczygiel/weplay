#include "../State.hpp"
#include "raylib.h"
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

const float FRICTION = 0.8F;

class Pong final : public State {

public:
    Pong()
        : m_width { static_cast<float>(GetScreenWidth()) }
        , m_height { static_cast<float>(GetScreenHeight()) }
        , m_animation_timer { 0.0F }
        , m_animation_timer_shift { 0.0F }
        , m_framebuffer { LoadRenderTexture(static_cast<int>(m_width), static_cast<int>(m_height)) }
        , m_score_position {}
    {
        m_racket_height = 0.25F * m_height;
        m_racket_width = 0.1F * m_racket_height;
        m_ball_radius = 0.65F * m_racket_width;
        m_player_speed_factor = 0.75F * m_height;
        m_ball_speed_factor = 0.65F * sqrtf(m_width * m_width + m_height * m_width);
        m_collision_shift = 0.5F * m_ball_radius;
        m_font_size = static_cast<int>(0.05F * m_height);
        m_animation_font_size = static_cast<int>(0.15F * m_height);

        restart();
    }

    ~Pong() override = default;

    void update(const std::vector<ControllerState>& controllers) override;
    void draw() override;
    RenderTexture2D framebuffer() override;
    StateChange state_change() override { return m_state_change; }

private:
    enum class State { PLAYING, SCORING };

    enum class AnimationState { SHADOWING, LIGHTING, NONE };

    float m_width;
    float m_height;
    float m_racket_width;
    float m_racket_height;
    float m_ball_radius;
    float m_player_speed_factor;
    float m_ball_speed_factor;
    float m_collision_shift;
    float m_animation_timer;
    float m_animation_timer_shift;

    int m_font_size;
    int m_animation_font_size;
    int m_score_position;

    RenderTexture2D m_framebuffer {};

    StateChange m_state_change { StateChange::None };

    State m_game_state { State::PLAYING };

    AnimationState m_anim_state { AnimationState::SHADOWING };

    Player m_player_1 {};
    Player m_player_2 {};

    Ball m_ball {};

    const char* m_score {};

    std::mt19937 m_gen { std::random_device {}() };

    void restart();
    void add_score(int id);
    int text_position_center(const char* text, int font_size);

    static Vector2 compute_ball_speed(Vector2 v);
    static float tween(float value, float x);
};

}
