#include "../State.hpp"
#include <raylib.h>

namespace Menu {

class Menu final : public State {
public:
    Menu()
        : m_framebuffer { LoadRenderTexture(1024, 768) }
    {
    }

    ~Menu() override { UnloadRenderTexture(m_framebuffer); }

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
};

}
