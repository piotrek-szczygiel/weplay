#pragma once
#include "Action.hpp"
#include <unordered_map>
#include <vector>

namespace Tetris {

constexpr double DELAY = 0.150;
constexpr double REPEAT = 0.050;

struct KeyBind {
    Action action;
    bool repeat;

    KeyBind(Action action, bool repeat)
        : action(action)
        , repeat(repeat)
    {
    }
};

class Input {
public:
    Input& bind(KeyBind bind);
    Input& bind(Action action, bool repeat) { return bind({ action, repeat }); }

    std::vector<Action> update(float dt, std::vector<Action> wanted);

private:
    std::vector<KeyBind> m_binds {};
    std::unordered_map<Action, float> m_action_activated {};
    std::unordered_map<Action, float> m_action_repeated {};
};

}
