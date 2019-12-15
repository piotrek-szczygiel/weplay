#pragma once
#include <map>
#include <mutex>
#include <vector>

using namespace std::chrono_literals;
using Timestamp = std::chrono::time_point<std::chrono::system_clock>;

struct ControllerState {
    bool connected { false };
    Timestamp last_update {};

    std::array<bool, 16> buttons { false };
    int16_t yaw {};
    int16_t pitch {};
    int16_t roll {};
};

class Controllers {
public:
    bool set_state(const std::string& id, const ControllerState& state)
    {
        std::lock_guard<std::mutex> guard { m_mutex };
        bool new_controller = (m_controllers.find(id) == m_controllers.end());
        m_controllers[id] = state;
        return new_controller;
    }

    std::vector<ControllerState> get_states()
    {
        std::lock_guard<std::mutex> guard { m_mutex };
        std::vector<ControllerState> states {};
        for (const auto& c : m_controllers) {
            states.push_back(c.second);
        }

        ControllerState dummy {};

        if (m_controllers.size() < 2) {
            states.push_back(dummy);
        }
        if (m_controllers.empty()) {
            states.push_back(dummy);
        }

        return states;
    }

    size_t remove_timed_out(Timestamp now, std::chrono::seconds timeout)
    {
        std::lock_guard<std::mutex> guard { m_mutex };
        size_t removed = 0;

        for (auto it = m_controllers.begin(); it != m_controllers.end();) {
            if (now - it->second.last_update > timeout) {
                it = m_controllers.erase(it);
                ++removed;
            } else {
                ++it;
            }
        }

        return removed;
    }

private:
    std::mutex m_mutex;
    std::map<std::string, ControllerState> m_controllers {};
};
