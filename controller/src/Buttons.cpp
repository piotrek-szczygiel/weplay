#include "Buttons.hpp"
#include <Arduino.h>

void Buttons::initialize()
{
    m_mcp.begin(7);

    for (uint8_t i = 0; i < 8; ++i) {
        m_mcp.pinMode(i, INPUT);
        m_mcp.pullUp(i, HIGH);
    }
}

bool Buttons::update()
{
    m_state = {};
    for (uint8_t i = 0; i < 8; ++i) {
        m_state |= m_mcp.digitalRead(i) << i;
    }

    if (m_state != m_last_state) {
        m_last_state = m_state;
        return true;
    }

    return false;
}
