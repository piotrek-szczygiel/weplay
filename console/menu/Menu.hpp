#include "../State.hpp"
#include <raylib.h>

namespace Menu {

constexpr int GAMES { 3 };
constexpr float SLIDE_SPEED {1.5F};

class Menu final : public State {
public:
    Menu()
        : m_framebuffer { LoadRenderTexture(1024, 768) }
        , m_bg { LoadTexture("resources/bg.png") }
        , m_shader { LoadShader(nullptr, "resources/wave.frag") }
        , m_width { GetScreenWidth() }
        , m_height { GetScreenHeight() }
    {
        m_seconds_loc = GetShaderLocation(m_shader, "seconds");

        float screenSize[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
        SetShaderValue(m_shader, GetShaderLocation(m_shader, "size"), &screenSize, UNIFORM_VEC2);

        m_games_images[0] = LoadTexture("resources/menu/starship.png");
        m_games_images[1] = LoadTexture("resources/menu/tetris.png");
        m_games_images[2] = LoadTexture("resources/menu/pong.png");

        m_seconds = 0.0F;
    }

    ~Menu() override
    {
        UnloadRenderTexture(m_framebuffer);
        UnloadTexture(m_bg);
        UnloadShader(m_shader);
    }

    void update(std::shared_ptr<ControllerState> state) override;
    void draw() override;
    RenderTexture2D framebuffer() override;
    StateChange state_change() override { return m_state_change; }
    void draw_game_name(int font_size);
    void draw_game_image(float dt);

private:
    RenderTexture2D m_framebuffer;
    StateChange m_state_change { StateChange::None };

    int16_t m_yaw {};
    int16_t m_pitch {};
    int16_t m_roll {};

    std::array<bool, 16> m_buttons {};

    Texture2D m_bg;
    Shader m_shader;
    int m_seconds_loc;
    float m_seconds;

    // main menu games images / animation
    enum AnimationState { PLAYING_LEFT, PLAYING_RIGHT, ENDING, NONE };

    std::string m_games_names[GAMES] { "Starship", "Tetris", "Pong" };
    Texture m_games_images[GAMES] {};
    StateChange m_games_states[GAMES] {StateChange::Starship, StateChange ::Tetris, StateChange ::Pong};
    int m_game_index {};
    int m_last_game_index {};
    int m_game_name_position {};
    int m_width;
    int m_height;
    AnimationState m_animation_state { NONE };
    float m_animation_timer {};
};

}
