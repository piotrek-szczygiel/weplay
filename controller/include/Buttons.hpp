#pragma once

#include <Adafruit_MCP23017.h>

class Buttons {
public:
    void initialize();
    bool update();
    uint8_t state() { return m_state; }

private:
    Adafruit_MCP23017 m_mcp {};

    uint8_t m_state {};
    uint8_t m_last_state {};
};