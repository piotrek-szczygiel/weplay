#pragma once

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
}
