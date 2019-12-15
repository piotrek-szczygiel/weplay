#include "../State.hpp"
#include <algorithm>
#include <array>
#include <filesystem>
#include <raylib.h>

namespace Menu {

constexpr int GAMES { 3 };
constexpr float SLIDE_SPEED { 2.0F };

enum class AnimationState {
    NONE,
    PLAYING_LEFT,
    PLAYING_RIGHT,
};

class Menu final : public State {
public:
    Menu()
        : m_width { 1024 }
        , m_height { 768 }
        , m_framebuffer { LoadRenderTexture(m_width, m_height) }
    {
        m_games_images[0] = LoadTexture("resources/menu/starship.png");
        m_games_images[1] = LoadTexture("resources/menu/tetris.png");
        m_games_images[2] = LoadTexture("resources/menu/pong.png");

        std::vector<std::string> background {};
        for (auto& p : std::filesystem::directory_iterator("resources/menu/wallpaper")) {
            background.push_back(p.path().string());
        }

        std::sort(background.begin(), background.end());
        for (auto& p : background) {
            m_background.push_back(LoadTexture(p.c_str()));
        }
    }

    ~Menu() override
    {
        for (auto& t : m_background) {
            UnloadTexture(t);
        }

        UnloadTexture(m_games_images[2]);
        UnloadTexture(m_games_images[1]);
        UnloadTexture(m_games_images[0]);

        UnloadRenderTexture(m_framebuffer);
    }

    void update() override;
    void draw() override;
    RenderTexture2D framebuffer() override;
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
    AnimationState m_animation_state { AnimationState::NONE };
    float m_animation_timer {};
};

}
