#include "Input.hpp"
#include <algorithm>
#include <raylib.h>

namespace Tetris {

Input& Input::bind(KeyBind bind)
{
    m_binds.push_back(bind);
    return *this;
}

std::vector<Action> Input::update(float dt, std::vector<Action> wanted)
{
    std::vector<Action> result {};

    for (KeyBind bind : m_binds) {
        if (std::find(wanted.begin(), wanted.end(), bind.action) == wanted.end()) {
            m_action_activated.erase(bind.action);
            m_action_repeated.erase(bind.action);
            continue;
        }

        bool active = false;

        auto activated = m_action_activated.find(bind.action);
        auto repeated = m_action_repeated.find(bind.action);

        if (activated == m_action_activated.end()) {
            m_action_activated.emplace(bind.action, 0.0F);
            active = true;
        } else {
            activated->second += dt;
            if (bind.repeat && activated->second > DELAY) {
                if (repeated == m_action_repeated.end()) {
                    m_action_repeated.emplace(bind.action, 0.0F);
                    active = true;
                } else {
                    repeated->second += dt;
                    if (repeated->second > REPEAT) {
                        m_action_repeated[bind.action] = 0.0F;
                        active = true;
                    }
                }
            }
        }

        if (active) {
            result.push_back(bind.action);
        }
    }

    return result;
}

}
