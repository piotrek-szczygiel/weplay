#include "../state.hpp"
#include <raylib.h>

class Menu final : public State {
private:
    RenderTexture2D framebuffer;

public:
    Menu()
        : framebuffer { LoadRenderTexture(GetScreenWidth(), GetScreenHeight()) }
    {
    }

    ~Menu()
    {
        UnloadRenderTexture(framebuffer);
    }

    void update(std::shared_ptr<Controller::State> state) override;
    void draw() override;
    RenderTexture2D get_framebuffer() override;
};
