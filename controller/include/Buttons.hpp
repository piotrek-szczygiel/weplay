#pragma once

#include <Adafruit_MCP23017.h>

class Buttons {
public:
    void initialize();
    bool update();

    uint16_t state() const { return m_state; }
    bool down(uint8_t pin) const { return (m_state & 1 << pin) == 0; }
    void print_status() const;

private:
    Adafruit_MCP23017 m_mcp {};

    uint16_t m_state {};
    uint16_t m_last_state {};
};
