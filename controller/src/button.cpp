#include "button.hpp"

bool Button::read()
{
    state = digitalRead(pin);
    if (state != lastState) {
        lastState = state;
        return true;
    }

    return false;
}
