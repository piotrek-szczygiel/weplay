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
    if (m_state == GAME_OVER || m_state == GAME_OVER_ANIMATION) {
        return;
    }

    change_state(CHANGING_LEVEL);
    m_changing_level_duration = {};

    m_level += 1;
    if (m_level > m_max_level) {
        m_level = m_max_level;
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
                change_state(CLEARING);
                int count = static_cast<int>(rows.size());
                m_clearing_rows = std::move(rows);
                m_clearing_duration = {};
                m_score.update_clear(m_level, count, false);
                m_lines += m_score.lines();
            } else {
                m_score.reset_combo();
            }
        }
        break;
    }
    case Action::GAME_OVER: {
        change_state(GAME_OVER_ANIMATION);
        m_game_over_duration = {};
        m_game_over_row = {};
        break;
    }
    default:
        break;
    }
}

void Player::change_state(State state)
{
    if (m_state == GAME_OVER) {
        return;
    }

    m_last_state = m_state;
    m_state = state;
}

void Player::update(float dt, const std::vector<Action>& actions)
{
    if (m_state == GAME_OVER) {
        return;
    }

    if (m_state == GAME_OVER_ANIMATION) {
        m_game_over_duration += dt;
        if (m_game_over_duration >= m_game_over_max_duration) {
            change_state(GAME_OVER);
        } else {
            int expected_row = static_cast<int>(
                m_game_over_duration / m_game_over_max_duration * static_cast<float>(HEIGHT + 2));

            int rows = expected_row - m_game_over_row;

            for (int i = 0; i < rows; ++i) {
                m_matrix.add_garbage_line(-1);
            }

            m_game_over_row = expected_row;
        }
    }

    if (m_state == CLEARING) {
        m_clearing_duration += dt;
        if (m_clearing_duration >= m_clearing_max_duration) {
            m_matrix.clear_rows(m_clearing_rows);

            change_state(PLAYING);
        }
    }

    if (m_state == CHANGING_LEVEL) {
        m_changing_level_duration += dt;
        if (m_changing_level_duration >= m_changing_level_max_duration) {
            if (m_last_state == CHANGING_LEVEL) {
                change_state(PLAYING);
            } else {
                change_state(m_last_state);
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

void Player::draw(int draw_x, int draw_y, bool left)
{
    m_matrix.draw(m_level, draw_x, draw_y);

    if (m_state == PLAYING || m_state == CLEARING || m_state == CHANGING_LEVEL) {
        m_ghost.draw(m_level, draw_x, draw_y, false, true);
        m_piece.draw(m_level, draw_x, draw_y, false, false);
    }

    if (m_state == GAME_OVER) {
        DrawText("Game", draw_x + 4, draw_y + 34, 30, BLACK);
        DrawText("Game", draw_x + 5, draw_y + 35, 30, MAROON);

        DrawText("Over", draw_x + 4, draw_y + 70, 30, BLACK);
        DrawText("Over", draw_x + 5, draw_y + 71, 30, MAROON);
    }

    if (m_state == CLEARING || (m_state == CHANGING_LEVEL && m_last_state == CLEARING)) {
        float cover
            = m_clearing_duration / m_clearing_max_duration * static_cast<float>(WIDTH * TILE_SIZE);

        for (int row : m_clearing_rows) {
            Rectangle rect {
                static_cast<float>(draw_x) + static_cast<float>(WIDTH * TILE_SIZE) / 2.0F,
                static_cast<float>(draw_y + (row - VANISH) * TILE_SIZE)
                    + static_cast<float>(TILE_SIZE) / 2.0F,
                cover,
                static_cast<float>(TILE_SIZE),
            };

            DrawRectanglePro(rect, { rect.width / 2.0F, rect.height / 2.0F }, {}, BLACK);
        }
    }

    if (m_state == CHANGING_LEVEL) {
        int expected_flash
            = static_cast<int>(m_changing_level_duration / m_changing_level_max_duration * 10.0F);

        if (expected_flash % 2 == 0) {
            DrawRectangle(
                draw_x - 1, draw_y - 2, WIDTH * TILE_SIZE + 1, HEIGHT * TILE_SIZE + 2, RAYWHITE);
        }
    }

    m_score.draw(draw_x + 1, draw_y - 26);

    auto next = shape_from_type(m_bag.peek());
    int next_x {};

    int next_offset_x = (34 - next.grids[0].width * TILE_SIZE_SMALL) / 2;
    int next_offset_y
        = (24 - next.grids[0].height * TILE_SIZE_SMALL) / 2 - next.grids[0].y * TILE_SIZE_SMALL;

    if (left) {
        next_x = draw_x - 47 + next_offset_x;
    } else {
        next_x = draw_x + WIDTH * TILE_SIZE + 13 + next_offset_x;
    }
    next.draw(m_level, next_x, draw_y + 7 + next_offset_y, 0, true, false);

    if (m_hold) {
        auto hold = shape_from_type(m_hold.value());
        int hold_x {};

        int hold_offset_x = (34 - hold.grids[0].width * TILE_SIZE_SMALL) / 2;
        int hold_offset_y
            = (24 - hold.grids[0].height * TILE_SIZE_SMALL) / 2 - hold.grids[0].y * TILE_SIZE_SMALL;

        if (left) {
            hold_x = draw_x - 47 + hold_offset_x;
        } else {
            hold_x = draw_x + WIDTH * TILE_SIZE + 13 + hold_offset_x;
        }

        hold.draw(m_level, hold_x, draw_y + 53 + hold_offset_y, 0, true, false);
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
