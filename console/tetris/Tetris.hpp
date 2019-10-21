#include "../State.hpp"
#include "Matrix.hpp"
#include "Piece.hpp"
#include <raylib.h>

namespace Tetris {

class Tetris final : public State {
public:
    Tetris()
        : framebuffer_ { LoadRenderTexture(320, 240) }
        , piece_ { SHAPE_T }
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
    RenderTexture2D framebuffer_;

    Matrix matrix_;
    Piece piece_;
};

}
