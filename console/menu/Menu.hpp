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

private:
    RenderTexture2D m_framebuffer;

    int16_t m_yaw {};
    int16_t m_pitch {};
    int16_t m_roll {};

    std::array<bool, 8> m_buttons {};
};

}
