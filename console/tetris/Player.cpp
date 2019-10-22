#include "Player.hpp"
#include <boost/log/trivial.hpp>

namespace Tetris {

Player::Player()
{
    m_piece = { m_bag.pop() };

    m_input.bind(Action::MOVE_LEFT, true)
        .bind(Action::MOVE_RIGHT, true)
        .bind(Action::SOFT_DROP, true)
        .bind(Action::HARD_DROP, false)
        .bind(Action::ROTATE_CLOCKWISE, false)
        .bind(Action::ROTATE_COUNTER_CLOCKWISE, false);
}

void Player::action(Action a)
{
    if (m_game_over) {
        return;
    }

    auto collision = std::bind(&Matrix::collision, m_matrix, std::placeholders::_1);

    switch (a) {
    case Action::MOVE_LEFT:
        m_piece.move(-1, 0, collision);
        break;
    case Action::MOVE_RIGHT:
        m_piece.move(1, 0, collision);
        break;
    case Action::SOFT_DROP:
        if (m_piece.move(0, 1, collision)) {
            reset_fall();
        }
        break;
    case Action::HARD_DROP:
        m_piece.fall(collision);
        action(Action::LOCK);
        break;
    case Action::ROTATE_CLOCKWISE:
        m_piece.rotate(true, collision);
        break;
    case Action::ROTATE_COUNTER_CLOCKWISE:
        m_piece.rotate(false, collision);
        break;
    case Action::FALL:
        if (!m_piece.move(0, 1, collision)) {
            action(Action::LOCK);
        }
        break;
    case Action::LOCK:
        if (!m_matrix.lock(m_piece)) {
            action(Action::GAME_OVER);
        } else {
            new_piece();
        }
        break;
    case Action::GAME_OVER:
        m_game_over = true;
        break;
    default:
        break;
    }
}

void Player::update(float dt, std::vector<Action> actions)
{
    if (m_game_over) {
        return;
    }

    for (Action a : m_input.update(std::move(actions))) {
        action(a);
    }

    m_falling += dt;

    if (m_falling > 1.0F) {
        m_falling -= 1.0F;

        action(Action::FALL);
    }
}

void Player::draw(int draw_x, int draw_y)
{
    m_piece.draw(draw_x, draw_y);
    m_matrix.draw(draw_x, draw_y);
}

void Player::new_piece()
{
    m_piece = { m_bag.pop() };
    reset_fall();
}

void Player::reset_fall() { m_falling = 0.0F; }

}
