#include "Player.hpp"
#include <boost/log/trivial.hpp>

namespace Tetris {

void Player::action(Action a)
{
    if (gameOver_) {
        return;
    }

    auto collision = std::bind(&Matrix::collision, matrix_, std::placeholders::_1);

    switch (a) {
    case Action::MOVE_LEFT:
        piece_.move(-1, 0, collision);
        break;
    case Action::MOVE_RIGHT:
        piece_.move(1, 0, collision);
        break;
    case Action::SOFT_DROP:
        if (piece_.move(0, 1, collision)) {
            resetFall_();
        }
        break;
    case Action::HARD_DROP:
        piece_.fall(collision);
        action(Action::LOCK);
        break;
    case Action::ROTATE_CLOCKWISE:
        piece_.rotate(true, collision);
        break;
    case Action::ROTATE_COUNTER_CLOCKWISE:
        piece_.rotate(false, collision);
        break;
    case Action::FALL:
        if (!piece_.move(0, 1, collision)) {
            action(Action::LOCK);
        }
        break;
    case Action::LOCK:
        if (!matrix_.lock(piece_)) {
            action(Action::GAME_OVER);
        } else {
            newPiece_();
        }
        break;
    case Action::GAME_OVER:
        gameOver_ = true;
        break;
    default:
        break;
    }
}

void Player::update(float dt)
{
    if (gameOver_) {
        BOOST_LOG_TRIVIAL(debug) << "game over";
        return;
    }

    falling_ += dt;

    if (falling_ > 1.0F) {
        falling_ -= 1.0F;

        action(Action::FALL);
    }
}

void Player::draw(int drawX, int drawY)
{
    piece_.draw(drawX, drawY);
    matrix_.draw(drawX, drawY);
}

void Player::newPiece_()
{
    piece_ = { bag_.pop() };
    resetFall_();
}

void Player::resetFall_()
{
    falling_ = 0.0F;
}

}
