#include "Player.hpp"
#include "DrawBlock.hpp"

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
        .bind(Action::ROTATE_LEFT, false)
        .bind(Action::HOLD, false);
}

void Player::increase_level()
{
    m_level_increased += 1;
    if (m_level_increased > m_max_level) {
        m_level_increased = m_max_level;
    }
}

void Player::action(Action a)
{
    switch (a) {
    case Action::MOVE_LEFT: {
        if (m_piece.move(-1, 0, collision()) && m_piece.touching_floor(collision())) {
            reset_fall();
        }
        break;
    }
    case Action::MOVE_RIGHT: {
        if (m_piece.move(1, 0, collision()) && m_piece.touching_floor(collision())) {
            reset_fall();
        }
        break;
    }
    case Action::MOVE_DOWN: {
        if (m_piece.move(0, 1, collision())) {
            reset_fall();
        }
        break;
    }
    case Action::SOFT_DROP: {
        int rows = m_piece.fall(collision());
        if (rows > 0) {
            m_score.soft_drop(rows);
            reset_fall();
        }
        break;
    }
    case Action::HARD_DROP: {
        int rows = m_piece.fall(collision());
        m_score.hard_drop(rows);
        action(Action::LOCK);
        break;
    }
    case Action::ROTATE_RIGHT: {
        if (m_piece.rotate(true, collision()) && m_piece.touching_floor(collision())) {
            reset_fall();
        }
        break;
    }
    case Action::ROTATE_LEFT: {
        if (m_piece.rotate(false, collision()) && m_piece.touching_floor(collision())) {
            reset_fall();
        }
        break;
    }
    case Action::HOLD: {
        if (m_hold_lock) {
            break;
        }

        m_hold_lock = true;

        if (m_hold) {
            ShapeType hold = m_hold.value();
            ShapeType current = m_piece.type();

            m_piece = { hold };
            m_hold = { current };
        } else {
            m_hold = { m_piece.type() };
            new_piece();
        }
        break;
    }
    case Action::FALL: {
        if (!m_piece.move(0, 1, collision())) {
            action(Action::LOCK);
        }
        break;
    }
    case Action::LOCK: {
        if (!m_matrix.lock(m_piece)) {
            action(Action::GAME_OVER);
        } else {
            new_piece();
            m_hold_lock = false;

            auto rows = m_matrix.get_full_rows();
            if (!rows.empty()) {
                m_state = CLEARING;
                int count = static_cast<int>(rows.size());
                m_clearing_rows = std::move(rows);
                m_clearing_duration = {};
                m_score.update_clear(m_level, count, false);
            } else {
                m_score.reset_combo();
            }

            m_level = m_level_increased;
        }
        break;
    }
    case Action::GAME_OVER: {
        m_state = GAME_OVER_ANIMATION;
        for (int y = VANISH - 1; y < VANISH + HEIGHT; ++y) {
            m_clearing_rows.push_back(y);
        }
        m_clearing_duration = {};
        m_clearing_max_duration = 0.75F;
        break;
    }
    default:
        break;
    }
}

void Player::update(float dt, const std::vector<Action>& actions)
{
    if (m_state == GAME_OVER) {
        return;
    }

    if (m_state == CLEARING || m_state == GAME_OVER_ANIMATION) {
        m_clearing_duration += dt;
        if (m_clearing_duration >= m_clearing_max_duration) {
            m_matrix.clear_rows(m_clearing_rows);

            if (m_state == CLEARING) {
                m_state = PLAYING;
            } else if (m_state == GAME_OVER_ANIMATION) {
                m_state = GAME_OVER;
            }
        }
    }

    if (m_state == PLAYING) {
        for (Action a : m_input.update(dt, actions)) {
            action(a);
        }

        m_falling += dt;

        auto interval = m_gravity[m_level - 1];
        if (m_falling >= interval) {
            m_falling -= interval;

            action(Action::FALL);
        }

        m_ghost = m_piece;
        m_ghost.fall(collision());
    }
}

void Player::draw(int draw_x, int draw_y)
{
    m_matrix.draw(m_level, draw_x, draw_y);

    if (m_state == PLAYING || m_state == CLEARING) {
        m_ghost.draw(m_level, draw_x, draw_y, false, true);
        m_piece.draw(m_level, draw_x, draw_y, false, false);
    }

    if (m_state == GAME_OVER) {
        RlDrawText("Game Over", draw_x + 10, draw_y + 72, 12, MAROON);
    }

    if (m_state == CLEARING || m_state == GAME_OVER_ANIMATION) {
        float cover
            = m_clearing_duration / m_clearing_max_duration * static_cast<float>(WIDTH * TILE_SIZE);

        for (int row : m_clearing_rows) {
            RlRectangle rect {
                static_cast<float>(draw_x) + static_cast<float>(WIDTH * TILE_SIZE) / 2.0F,
                static_cast<float>(draw_y + (row - VANISH) * TILE_SIZE)
                    + static_cast<float>(TILE_SIZE) / 2.0F,
                cover,
                static_cast<float>(TILE_SIZE),
            };

            DrawRectanglePro(rect, { rect.width / 2.0F, rect.height / 2.0F }, {}, BLACK);
        }
    }

    m_matrix.draw_outline(draw_x, draw_y);

    m_score.draw(draw_x, draw_y - 20, 8);

    RlDrawText("next", draw_x + WIDTH * TILE_SIZE + 6, draw_y, 6, RAYWHITE);
    shape_from_type(m_bag.peek())
        .draw(m_level, draw_x + WIDTH * TILE_SIZE + 6, draw_y + 12, 0, true, false);

    RlDrawText("hold", draw_x - 5 * TILE_SIZE_SMALL, draw_y, 6, RAYWHITE);
    if (m_hold) {
        shape_from_type(m_hold.value())
            .draw(m_level, draw_x - 5 * TILE_SIZE_SMALL, draw_y + 2 * TILE_SIZE_SMALL, 0, true,
                false);
    }
}

void Player::new_piece()
{
    m_piece = { m_bag.pop() };
    if (m_matrix.collision(m_piece)) {
        action(Action::GAME_OVER);
    }

    reset_fall();
}

void Player::reset_fall() { m_falling = 0.0F; }

Piece::CollisionFunction Player::collision()
{
    return std::bind(&Matrix::collision, m_matrix, std::placeholders::_1);
}

}
