#pragma once
#include "Action.hpp"
#include "Bag.hpp"
#include "Input.hpp"
#include "Matrix.hpp"
#include "Piece.hpp"
#include "Score.hpp"
#include <memory>
#include <optional>

namespace Tetris {

class Player {
public:
    Player();

    void increase_level();

    void update(float dt, const std::vector<Action>& actions);
    void draw(int draw_x, int draw_y);

private:
    enum State {
        PLAYING,
        CLEARING,
        GAME_OVER_ANIMATION,
        GAME_OVER,
    };

    State m_state { PLAYING };
    Piece m_piece { O };
    Piece m_ghost { O };
    Matrix m_matrix {};
    Bag m_bag {};
    Input m_input {};
    Score m_score {};

    std::optional<ShapeType> m_hold {};
    bool m_hold_lock {};

    std::vector<int> m_clearing_rows {};
    float m_clearing_duration {};
    float m_clearing_max_duration { 0.3F };

    float m_falling {};
    int m_level { 1 };
    int m_level_increased { 1 };

    static constexpr int m_max_level { 10 };
    static constexpr std::array<float, m_max_level> m_gravity { {
        1.00000F,
        0.79300F,
        0.61780F,
        0.47273F,
        0.35520F,
        0.26200F,
        0.18968F,
        0.13473F,
        0.09388F,
        0.06415F,
    } };

    void action(Action action);

    void new_piece();
    void reset_fall();
    Piece::CollisionFunction collision();
};

}
