#include "Connection.hpp"
#include "Mpu.hpp"
#include <Adafruit_MCP23017.h>
#include <string>

Connection connection { F("RaspberryConsole"), F("Korobeiniki1984"), F("192.168.12.1"), 1984 };

Adafruit_MCP23017 mcp;

Calibration calibration { 1560, 1367, 626, 62, 86, -54 };
Mpu mpu { calibration };

unsigned long last_send {};

int last_button_status = HIGH;

void setup()
{
    Serial.begin(115200);
    while (!Serial) { };

    mcp.begin(7);
    mcp.pinMode(0, INPUT);
    mcp.pullUp(0, HIGH);

    mpu.initialize();
}

void loop()
{
    connection.connect();

    if (mpu.update()) {
        Serial.printf("ypr\t%d\t%d\t%d\r\n", mpu.yaw(), mpu.pitch(), mpu.roll());
        connection.send_ypr(mpu.yaw(), mpu.pitch(), mpu.roll());
    }

    int button_status = mcp.digitalRead(0);
    if (button_status != last_button_status) {
        Serial.printf("Button: %d\r\n", button_status);
        last_button_status = button_status;
    }
}
