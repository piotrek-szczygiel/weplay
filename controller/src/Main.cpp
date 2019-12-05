#include "Buttons.hpp"
#include "Connection.hpp"
#include "Mpu.hpp"
#include "Print.hpp"
#include <Ticker.h>

Buttons buttons {};
Mpu mpu {};

Connection connection {
    "matebook",    // SSID
    "korobeiniki", // Password
    1984,          // Port
};

void ping() { connection.ping(); }
Ticker keep_alive { ping, 1000 };

// Switch to true if you want to calibrate a controller.
const bool NEW_CALIBRATION { false };

const Calibration CONTROLLER_1 { 1620, 1393, 584, 40, 110, -41 };
const Calibration CONTROLLER_2 { -3946, 2651, 1802, 22, 54, 54 };

// Switch to tru if you want to see realtime MPU output
const bool DEBUG_MPU { false };

void setup()
{
    init_print();
    println("Raspberry controller initialized");

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

    if (!mpu.initialize()) {
        while (true) {
            delay(500);
        }
    }

    keep_alive.start();
}

void loop()
{
    connection.connect();

    if (buttons.update()) {
        connection.send_buttons(buttons.state());
        buttons.print_status();
    }

    if (mpu.update()) {
        connection.send_ypr(mpu.yaw(), mpu.pitch(), mpu.roll());

        if (DEBUG_MPU) {
            mpu.print_status();
        }
    }

    keep_alive.update();
}
