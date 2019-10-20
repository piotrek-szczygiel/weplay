#include "../State.hpp"
#include <raylib.h>

class Menu final : public State {
public:
    Menu()
        : framebuffer_ { LoadRenderTexture(GetScreenWidth(), GetScreenHeight()) }
    {
    }

    ~Menu() override
    {
        UnloadRenderTexture(framebuffer_);
    }

    void update(std::shared_ptr<Controller::State> state) override;
    void draw() override;
    RenderTexture2D getFramebuffer() override;

private:
    RenderTexture2D framebuffer_;
};
