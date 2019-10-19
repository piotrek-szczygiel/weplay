#include "../state.hpp"
#include <raylib.h>

class Tetris final : public State {
private:
    RenderTexture2D framebuffer;

public:
    Tetris()
        : framebuffer { LoadRenderTexture(GetScreenWidth(), GetScreenHeight()) }
    {
    }

    void update(std::shared_ptr<Controller::State> state) override;
    void draw() override;
    RenderTexture2D get_framebuffer() override;
};
