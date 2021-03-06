#include "../Config.hpp"
#include "../State.hpp"
#include <algorithm>
#include <array>
#include <filesystem>
#include <raylib.h>

namespace Menu {

constexpr int GAMES { 3 };
constexpr float ANIMATION_TIME { 0.5F };

enum class AnimationState {
    NONE,
    START_LEFT,
    START_RIGHT,
    PLAYING_LEFT,
    PLAYING_RIGHT,
};

struct Logo {
    int start_pos;
    int dest_pos;
    int draw_pos_x;
    int draw_pos_y;

    bool visible;

    StateChange game_state;

    Texture2D texture;

    std::string name;
};

class Menu final : public State {
public:
    Menu()
        : m_width { 1024 }
        , m_height { 768 }
        , m_game_logo_width { 256 }
        , m__game_logo_height { 256 }
        , m_framebuffer { LoadRenderTexture(m_width, m_height) }
    {
        int pos_y = m_height / 2 - m__game_logo_height / 2;

        m_logos[0] = {
            0,
            0,
            m_width / 2 - m_game_logo_width / 2,
            pos_y,
            true,
            StateChange::Starship,
            LoadTexture("resources/menu/starships.png"),
            "Starship",
        };

        m_logos[1] = {
            0,
            0,
            0,
            pos_y,
            false,
            StateChange::Tetris,
            LoadTexture("resources/menu/tetris.png"),
            "Tetris",
        };

        m_logos[2] = {
            0,
            0,
            0,
            pos_y,
            false,
            StateChange::Pong,
            LoadTexture("resources/menu/ping_pong.png"),
            "Pong",
        };

        m_console_logo = LoadTexture("resources/menu/weplay_red.png");

        std::vector<std::string> background {};
        for (auto& p : std::filesystem::directory_iterator("resources/menu/wallpaper")) {
            background.push_back(p.path().string());
        }

        std::sort(background.begin(), background.end());
        for (auto& p : background) {
            m_background.push_back(LoadTexture(p.c_str()));
        }

        m_controller_icon = LoadTexture("resources/menu/controller.png");
    }

    ~Menu() override
    {
        for (auto& t : m_background) {
            UnloadTexture(t);
        }

        for (auto& logo : m_logos) {
            UnloadTexture(logo.texture);
        }

        UnloadTexture(m_console_logo);
        UnloadTexture(m_controller_icon);
        UnloadRenderTexture(m_framebuffer);
    }

    void update(const std::vector<ControllerState>& controllers) override;
    void draw() override;
    StateChange state_change() override { return m_state_change; }
    void draw_game_name(int font_size);
    void draw_game_image(float dt);

private:
    int m_width;
    int m_height;
    RenderTexture2D m_framebuffer;

    StateChange m_state_change { StateChange::None };

    size_t m_connected {};
    int16_t m_yaw {};
    int16_t m_pitch {};
    int16_t m_roll {};
    std::array<bool, 16> m_buttons {};

    std::vector<Texture2D> m_background {};
    float m_background_timer {};
    size_t m_background_frame {};

    std::array<std::string, GAMES> m_games_names { "Starship", "Tetris", "Pong" };
    std::array<Texture, GAMES> m_games_images {};

    StateChange m_games_states[GAMES] {
        StateChange::Starship,
        StateChange ::Tetris,
        StateChange ::Pong,
    };

    int m_game_index {};
    int m_last_game_index {};
    int m_game_name_position {};
    int m_game_logo_width {};
    int m__game_logo_height {};
    AnimationState m_animation_state { AnimationState::NONE };
    float m_animation_timer {};

    std::array<Logo, GAMES> m_logos;
    Texture2D m_console_logo;

    Texture2D m_controller_icon;
};
}
