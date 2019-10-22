#include "Player.hpp"
#include <boost/log/trivial.hpp>

namespace Tetris {

Player::Player()
{
    m_piece = { m_bag.pop() };

    m_input.bind(Action::MOVE_LEFT, true)
        .bind(Action::MOVE_RIGHT, true)
        .bind(Action::MOVE_DOWN, true)
        .bind(Action::SOFT_DROP, false)
        .bind(Action::HARD_DROP, false)
        .bind(Action::ROTATE_RIGHT, false)
        .bind(Action::ROTATE_LEFT, false);
}

void Player::action(Action a)
{
    auto collision = std::bind(&Matrix::collision, m_matrix, std::placeholders::_1);

    switch (a) {
    case Action::MOVE_LEFT:
        if (m_piece.move(-1, 0, collision) && m_piece.touching_floor(collision)) {
            reset_fall();
        }
        break;
    case Action::MOVE_RIGHT:
        if (m_piece.move(1, 0, collision) && m_piece.touching_floor(collision)) {
            reset_fall();
        }
        break;
    case Action::MOVE_DOWN:
        if (m_piece.move(0, 1, collision)) {
            reset_fall();
        }
        break;
    case Action::SOFT_DROP:
        if (m_piece.fall(collision) > 0) {
            reset_fall();
        }
        break;
    case Action::HARD_DROP:
        m_piece.fall(collision);
        action(Action::LOCK);
        break;
    case Action::ROTATE_RIGHT:
        if (m_piece.rotate(true, collision) && m_piece.touching_floor(collision)) {
            reset_fall();
        }
        break;
    case Action::ROTATE_LEFT:
        if (m_piece.rotate(false, collision) && m_piece.touching_floor(collision)) {
            reset_fall();
        }
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

            auto rows = m_matrix.get_full_rows();
            if (!rows.empty()) {
                m_state = CLEARING;
                m_clearing_rows = std::move(rows);
                m_clearing_duration = {};
            }
        }
        break;
    case Action::GAME_OVER:
        m_state = GAME_OVER;
        break;
    default:
        break;
    }
}

void Player::update(float dt, const std::vector<Action>& actions)
{
    if (m_state == GAME_OVER) {
        return;
    }

    if (m_state == CLEARING) {
        m_clearing_duration += dt;
        if (m_clearing_duration >= 0.5F) {
            m_state = PLAYING;
            m_matrix.clear_rows(m_clearing_rows);
        }
    }

    if (m_state == PLAYING) {
        for (Action a : m_input.update(dt, actions)) {
            action(a);
        }

        m_falling += dt;

        if (m_falling >= 1.0F) {
            m_falling -= 1.0F;

            action(Action::FALL);
        }
    }
}

void Player::draw(int draw_x, int draw_y)
{
    m_piece.draw(draw_x, draw_y);
    m_matrix.draw(draw_x, draw_y);

    if (m_state == CLEARING) {
        float cover = m_clearing_duration / 0.5F * static_cast<float>(WIDTH * BLOCK_SIZE);

        for (int row : m_clearing_rows) {
            RlRectangle rect {
                static_cast<float>(draw_x) + static_cast<float>(WIDTH * BLOCK_SIZE) / 2.0F,
                static_cast<float>(draw_y + (row - VANISH) * BLOCK_SIZE)
                    + static_cast<float>(BLOCK_SIZE) / 2.0F,
                cover,
                static_cast<float>(BLOCK_SIZE),
            };

            DrawRectanglePro(rect, { rect.width / 2.0F, rect.height / 2.0F }, {}, BLACK);
        }
    }
}

void Player::new_piece()
{
    m_piece = { m_bag.pop() };
    reset_fall();
}

void Player::reset_fall() { m_falling = 0.0F; }

}
