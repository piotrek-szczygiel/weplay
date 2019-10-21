#include "../State.hpp"
#include <raylib.h>

#include "Shape.hpp"
#include <boost/log/trivial.hpp>

class Tetris final : public State {
public:
    Tetris()
        : framebuffer_ { LoadRenderTexture(320, 240) }
    {
        for (const auto& rotationKick : kicksJLSTZ) {
            for (const auto& kick : rotationKick.clockwise) {
                BOOST_LOG_TRIVIAL(debug) << kick.x << "\t" << kick.y;
            }
        }
    }

    ~Tetris() override
    {
        UnloadRenderTexture(framebuffer_);
    }

    void update(std::shared_ptr<Controller::State> state) override;
    void draw() override;
    RenderTexture2D getFramebuffer() override;

private:
    RenderTexture2D framebuffer_;
};
