#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <string>

enum ButtonId : uint8_t {
    BUTTON_LEFT = '<',
    BUTTON_MIDDLE = '^',
    BUTTON_RIGHT = '>',
};

struct Button {
private:
    int lastState;

public:
    uint8_t pin;
    ButtonId id;

    uint8_t state;

    Button(uint8_t pin, ButtonId id)
        : pin(pin)
        , id(id)
    {
        pinMode(pin, INPUT_PULLUP);
        state = HIGH;
        lastState = HIGH;
    }

    bool read();
};

#endif
