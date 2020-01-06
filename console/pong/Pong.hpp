#include "../State.hpp"
#include "Ball.hpp"
#include "Player.hpp"
#include "PowerUp.hpp"
#include "raylib.h"
#include <charconv>
#include <random>

namespace Pong {

constexpr float POWER_UP_DELAY { 5.0F };

class Pong final : public State {

public:
    Pong()
        : m_width { 1024 }
        , m_height { 768 }
        , m_animation_timer { 0.0F }
        , m_animation_timer_shift { 0.0F }
        , m_framebuffer { LoadRenderTexture(m_width, m_height) }
        , m_score_position {}
    {
        m_font_size = static_cast<int>(0.05F * m_height);
        m_animation_font_size = static_cast<int>(0.15F * m_height);

        new_round();
    }

    ~Pong() override = default;

    void update(const std::vector<ControllerState>& controllers) override;
    void draw() override;
    StateChange state_change() override { return m_state_change; }

private:
    enum class State { PLAYING, SCORING };

    enum class AnimationState { SHADOWING_LAST_SCORE, LIGHTING, SHADOWING_SCORE, DELAY };

    int m_width;
    int m_height;

    int m_font_size;
    int m_animation_font_size;
    int m_score_position;

    float m_animation_timer;
    float m_animation_timer_shift;

    RenderTexture2D m_framebuffer {};

    StateChange m_state_change { StateChange::None };

    State m_game_state { State::PLAYING };

    AnimationState m_animation_state { AnimationState::SHADOWING_LAST_SCORE };

    Player m_player_1 {};
    Player m_player_2 {};

    Ball m_ball {};

    float m_power_up_timer { POWER_UP_DELAY };
    std::vector<PowerUp> m_power_ups;

    std::string m_score {};
    std::string m_last_score {};

    std::mt19937 m_gen { std::random_device {}() };
    std::uniform_real_distribution<> m_dis_real { 200.0F, 824.0F };
    std::uniform_int_distribution<> m_dis_int { 0, 1 };

    void new_round();
    void add_score(int id);
    int text_position_center(const char* text, int font_size);

    static float tween(float value, float x);
};
}
