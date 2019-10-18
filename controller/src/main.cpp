#include "button.hpp"
#include "connection.hpp"
#include <string>

std::array<Button, 3> buttons = {
    Button { 14, BUTTON_LEFT },
    Button { 12, BUTTON_MIDDLE },
    Button { 13, BUTTON_RIGHT },
};

Connection conn("RaspberryConsole", "Korobeiniki1984", "192.168.12.1", 1984);

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    conn.connect();

    for (auto& button : buttons) {
        if (button.read()) {
            conn.sendButton(button);
        }
    }
}
