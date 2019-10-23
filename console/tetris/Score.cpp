#include "Score.hpp"
#include <boost/format.hpp>
#include <raylib.h>
#include <string>

namespace Tetris {

void Score::update_clear(int level, int rows, bool t_spin)
{
    int lines {};
    int duel_lines {};

    if (rows == 1) {
        if (t_spin) {
            duel_lines = 2;
            if (m_last_clear_was_hard) {
                lines = 12;
            } else {
                lines = 8;
            }
        } else {
            lines = 1;
            duel_lines = 0;
        }
    } else if (rows == 2) {
        if (t_spin) {
            duel_lines = 4;
            if (m_last_clear_was_hard) {
                lines = 18;
            } else {
                lines = 12;
            }
        } else {
            lines = 3;
            duel_lines = 1;
        }
    } else if (rows == 3) {
        if (t_spin) {
            duel_lines = 6;
            if (m_last_clear_was_hard) {
                lines = 24;
            } else {
                lines = 16;
            }
        } else {
            lines = 5;
            duel_lines = 3;
        }
    } else if (rows == 4) {
        if (m_last_clear_was_hard) {
            lines = 12;
        } else {
            lines = 8;
        }

        duel_lines = 4;
    }

    if (m_last_clear_was_hard) {
        duel_lines += 1;
    }

    m_score += lines * 100 * level;
    m_lines = lines;
    m_duel_lines = duel_lines;
    m_last_clear_was_hard = t_spin || rows == 4;

    m_combo += 1;
    if (m_combo > 0) {
        m_score += 50 * m_combo * level;
    }
}

void Score::draw(int draw_x, int draw_y, int size)
{
    std::string str = boost::str(boost::format("Score: %1%") % m_score);
    RlDrawText(str.c_str(), draw_x, draw_y, size, RAYWHITE);
}

}
