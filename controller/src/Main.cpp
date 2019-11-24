#include "Buttons.hpp"
#include "Connection.hpp"
#include "Mpu.hpp"

Buttons buttons {};
Mpu mpu {};

Connection connection {
    F("RaspberryConsole"), // SSID
    F("Korobeiniki1984"),  // Password
    F("192.168.12.1"),     // IP Address
    1984,                  // TCP Port
};

// Switch to true if you want to calibrate a controller.
const bool NEW_CALIBRATION { false };

const Calibration CONTROLLER_1 { 1620, 1393, 584, 40, 110, -41 };
const Calibration CONTROLLER_2 { -3946, 2651, 1802, 22, 54, 54 };

void setup()
{
    Serial.begin(115200);
    while (!Serial) { }

    Serial.println();
    Serial.println("Controller program started");

    buttons.initialize();

    // Because every controller needs separate MPU calibration, we need
    // to detect which controller are we currently running this program on.
    // This is done by soldering expander's PB7 (15) pin to ground.
    if (!NEW_CALIBRATION) {
        if (!buttons.down(15)) {
            mpu.calibrate(CONTROLLER_1);
        } else {
            mpu.calibrate(CONTROLLER_2);
        }
    }

    mpu.initialize();
}

void loop()
{
    connection.connect();

    if (buttons.update()) {
        buttons.print_status();
        connection.send_buttons(buttons.state());
    }

    if (mpu.update()) {
        mpu.print_status();
        connection.send_ypr(mpu.yaw(), mpu.pitch(), mpu.roll());
    }
}
