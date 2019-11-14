#include "../State.hpp"
#include <raylib.h>

namespace Menu {

class Menu final : public State {
public:
    Menu()
        : m_framebuffer { LoadRenderTexture(1024, 768) }
        , m_bg { LoadTexture("resources/bg.png") }
        , m_shader { LoadShader(nullptr, "resources/wave.frag") }
    {
        m_seconds_loc = GetShaderLocation(m_shader, "seconds");

        float screenSize[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
        SetShaderValue(m_shader, GetShaderLocation(m_shader, "size"), &screenSize, UNIFORM_VEC2);

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
};

}
