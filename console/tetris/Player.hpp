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

    void action(Action action);
    void update(float dt, std::vector<Action> actions);
    void draw(int drawX, int drawY);

private:
    Piece piece_ { O };
    Matrix matrix_ {};
    Bag bag_ {};
    Input input_ {};

    float falling_ { 0.0F };
    bool gameOver_ { false };

    void newPiece_();
    void resetFall_();
};

}
