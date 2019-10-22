#include "Input.hpp"
#include <boost/log/trivial.hpp>
#include <raylib.h>

namespace Tetris {
Input& Input::bind(KeyBind bind)
{
    binds_.push_back(bind);
    return *this;
}

std::vector<Action> Input::update(std::vector<Action> wanted)
{
    std::vector<Action> activated {};
    double now = GetTime();

    for (KeyBind bind : binds_) {
        if (std::find(wanted.begin(), wanted.end(), bind.action) == wanted.end()) {
            actionActivated_.erase(bind.action);
            actionRepeated_.erase(bind.action);
            continue;
        }

        bool active = false;

        auto actionActivated = actionActivated_.find(bind.action);
        auto actionRepeated = actionRepeated_.find(bind.action);

        if (actionActivated == actionActivated_.end()) {
            actionActivated_.emplace(bind.action, now);
            active = true;
        } else if (bind.repeat && (now - actionActivated->second > DELAY)) {
            if (actionRepeated == actionRepeated_.end()) {
                actionRepeated_.emplace(bind.action, now);
                active = true;
            } else if (now - actionRepeated->second > REPEAT) {
                actionRepeated_[bind.action] = now;
                active = true;
            }
        }

        if (active) {
            activated.push_back(bind.action);
        }
    }

    return activated;
}
}
