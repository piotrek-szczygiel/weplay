#pragma once
#include "Action.hpp"
#include "Bag.hpp"
#include "Input.hpp"
#include "Matrix.hpp"
#include "Piece.hpp"
#include <memory>

namespace Tetris {

class Player {
public:
    Player();

    void update(float dt, std::vector<Action> actions);
    void draw(int draw_x, int draw_y);

private:
    enum State {
        PLAYING,
        CLEARING,
        GAME_OVER,
    };

    State m_state { PLAYING };
    Piece m_piece { O };
    Matrix m_matrix {};
    Bag m_bag {};
    Input m_input {};

    float m_falling { 0.0F };

    std::vector<int> m_clearing_rows {};
    double m_clearing_start {};

    void action(Action action);

    void new_piece();
    void reset_fall();
};

}
