#include "Input.hpp"
#include <boost/log/trivial.hpp>
#include <raylib.h>

namespace Tetris {

Input& Input::bind(KeyBind bind)
{
    m_binds.push_back(bind);
    return *this;
}

std::vector<Action> Input::update(std::vector<Action> wanted)
{
    std::vector<Action> result {};
    double now = GetTime();

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
            m_action_activated.emplace(bind.action, now);
            active = true;
        } else if (bind.repeat && (now - activated->second > DELAY)) {
            if (repeated == m_action_repeated.end()) {
                m_action_repeated.emplace(bind.action, now);
                active = true;
            } else if (now - repeated->second > REPEAT) {
                m_action_repeated[bind.action] = now;
                active = true;
            }
        }

        if (active) {
            result.push_back(bind.action);
        }
    }

    return result;
}

}
