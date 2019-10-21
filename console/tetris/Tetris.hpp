#include "../State.hpp"
#include "Player.hpp"
#include <memory>
#include <raylib.h>

namespace Tetris {

class Tetris final : public State {
public:
    Tetris()
        : framebuffer_ { LoadRenderTexture(width_, height_) }
    {
    }

    ~Tetris() override
    {
        UnloadRenderTexture(framebuffer_);
    }

    void update(std::shared_ptr<Controller::State> state) override;
    void draw() override;
    RenderTexture2D getFramebuffer() override;

private:
    const int width_ { 320 };
    const int height_ { 240 };
    RenderTexture2D framebuffer_;

    Player player1 {};
    Player player2 {};
};

}
