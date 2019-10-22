#include "../State.hpp"
#include <raylib.h>

namespace Menu {

class Menu final : public State {
public:
    Menu()
        : m_framebuffer { LoadRenderTexture(GetScreenWidth(), GetScreenHeight()) }
    {
    }

    ~Menu() override { UnloadRenderTexture(m_framebuffer); }

    void update(std::shared_ptr<ControllerState> state) override;
    void draw() override;
    RenderTexture2D framebuffer() override;

private:
    RenderTexture2D m_framebuffer;
};

}
