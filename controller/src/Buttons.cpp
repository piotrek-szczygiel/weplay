#include "Buttons.hpp"
#include <Arduino.h>

void Buttons::initialize()
{
    m_mcp.begin(7);

    for (uint8_t i = 0; i < 16; ++i) {
        m_mcp.pinMode(i, INPUT);
        m_mcp.pullUp(i, HIGH);
    }

    update();
}

bool Buttons::update()
{
    m_state = {};
    for (uint8_t i = 0; i < 16; ++i) {
        // The PA7 pin is always LOW on one of my controllers.
        // It is probably a factory issue, or it got damaged while soldering.
        if (i == 7) {
            m_state |= 1 << 7;
        } else {
            m_state |= m_mcp.digitalRead(i) << i;
        }
    }

    if (m_state != m_last_state) {
        m_last_state = m_state;
        return true;
    }

    return false;
}

void Buttons::print_status() const
{
    Serial.print(F("Buttons: "));

    for (uint8_t i = 0; i < 16; ++i) {
        Serial.print(m_state & 1 << i ? F(" ") : F("X"));
    }

    Serial.println();
}
