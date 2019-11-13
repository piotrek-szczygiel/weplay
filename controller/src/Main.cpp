#include "Buttons.hpp"
#include "Connection.hpp"
#include "Mpu.hpp"

#include <string>

Connection connection { F("RaspberryConsole"), F("Korobeiniki1984"), F("192.168.12.1"), 1984 };

Buttons buttons {};

Calibration calibration { 1560, 1367, 626, 62, 86, -54 };
Mpu mpu { calibration };

void setup()
{
    Serial.begin(115200);
    while (!Serial) { };

    buttons.initialize();
    mpu.initialize();
}

void loop()
{
    connection.connect();

    if (buttons.update()) {
        Serial.print("Buttons: ");
        uint16_t state = buttons.state();
        for (uint8_t i = 0; i < 16; ++i) {
            if (state & (1 << i)) {
                Serial.print(F(" "));
            } else {
                Serial.print(F("X"));
            }
        }
        Serial.println();

        connection.send_buttons(buttons.state());
    }

    if (mpu.update()) {
        Serial.printf(F("MPU6050: %d\t%d\t%d\r\n"), mpu.yaw(), mpu.pitch(), mpu.roll());

        connection.send_ypr(mpu.yaw(), mpu.pitch(), mpu.roll());
    }
}
