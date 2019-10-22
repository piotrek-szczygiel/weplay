#pragma once

namespace Tetris {

enum class Action {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_DOWN,
    ROTATE_RIGHT,
    ROTATE_LEFT,
    SOFT_DROP,
    HARD_DROP,
    HOLD,
    FALL,
    LOCK,
    GAME_OVER,
};

}
