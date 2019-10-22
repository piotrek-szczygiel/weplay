#pragma once

namespace Tetris {

class Score {
public:
    void soft_drop(int rows) { m_score += rows; }
    void hard_drop(int rows) { m_score += rows * 2; }
    void reset_combo() { m_combo = -1; }
    void update_clear(int level, int rows, bool t_spin);

    void draw(int draw_x, int draw_y, int size);

private:
    int m_score { 0 };
    int m_combo { -1 };
    bool m_last_clear_was_hard { false };

    int m_lines { 0 };
    int m_duel_lines { 0 };
};

}
