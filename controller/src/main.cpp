#include "Connection.hpp"
#include "Mpu.hpp"
#include <string>

Connection connection { F("RaspberryConsole"), F("Korobeiniki1984"), F("192.168.12.1"), 1984 };

Calibration calibration { 1560, 1367, 626, 62, 86, -54 };

Mpu mpu { calibration };

unsigned long last_send {};

void setup()
{
    Serial.begin(115200);
    while (!Serial) { };

    mpu.initialize();
}

void loop()
{
    connection.connect();

    if (mpu.update()) {
        Serial.printf("%d\t%d\t%d\r\n", mpu.yaw(), mpu.pitch(), mpu.roll());
        connection.send_ypr(mpu.yaw(), mpu.pitch(), mpu.roll());
    }
}
