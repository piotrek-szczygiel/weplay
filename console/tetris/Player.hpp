#pragma once
#include "Bag.hpp"
#include "Matrix.hpp"
#include "Piece.hpp"
#include <memory>

namespace Tetris {

enum class Action {
    MOVE_LEFT,
    MOVE_RIGHT,
    ROTATE_CLOCKWISE,
    ROTATE_COUNTER_CLOCKWISE,
    SOFT_DROP,
    HARD_DROP,
    FALL,
    LOCK,
    GAME_OVER,
};

class Player {
public:
    Player()
    {
        piece_ = { bag_.pop() };
    }

    void action(Action action);
    void update(float dt);
    void draw(int drawX, int drawY);

private:
    Piece piece_ { O };
    Matrix matrix_ {};
    Bag bag_ {};

    float falling_ { 0.0F };
    bool gameOver_ { false };

    void newPiece_();
    void resetFall_();
};

}
