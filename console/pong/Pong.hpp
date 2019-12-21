#include "../State.hpp"
#include "Ball.hpp"
#include "Player.hpp"
#include "raylib.h"
#include <charconv>

namespace Pong {

class Pong final : public State {

public:
    Pong()
        : m_width { GetScreenWidth() }
        , m_height { GetScreenHeight() }
        , m_animation_timer { 0.0F }
        , m_animation_timer_shift { 0.0F }
        , m_framebuffer { LoadRenderTexture(m_width, m_height) }
        , m_score_position {}
    {
        m_font_size = 0.05F * m_height;
        m_animation_font_size = 0.15F * m_height;

        new_round();
    }

    ~Pong() override = default;

    void update(const std::vector<ControllerState>& controllers) override;
    void draw() override;
    RenderTexture2D framebuffer() override;
    StateChange state_change() override { return m_state_change; }

private:
    enum class State { PLAYING, SCORING };

    enum class AnimationState { SHADOWING, LIGHTING, NONE };

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

    AnimationState m_anim_state { AnimationState::SHADOWING };

    Player m_player_1 {};
    Player m_player_2 {};

    Ball m_ball {};

    std::string m_score {};

    void new_round();
    void add_score(int id);
    int text_position_center(const char* text, int font_size);

    static float tween(float value, float x);
};

}
