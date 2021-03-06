#include "Buttons.hpp"
#include "Print.hpp"
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

void Buttons::update()
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
}

void Buttons::print_status() const
{
    print("Buttons: ");
    for (uint8_t i = 0; i < 16; ++i) {
        print("%c", m_state & 1 << i ? ' ' : 'X');
    }
    println();
}
