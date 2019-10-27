#pragma once

// Everything has to be in the header file,
// because splitting into source and header causes comiler errors.
// It is caused by poor implementation of MPU6050 library.
// https://github.com/jrowberg/i2cdevlib/issues/468

#include <MPU6050_6Axis_MotionApps20.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct Calibration {
    Calibration(int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz)
        : custom { true }
        , accel_x { ax }
        , accel_y { ay }
        , accel_z { az }
        , gyro_x { gx }
        , gyro_y { gy }
        , gyro_z { gz }
    {
    }

    Calibration()
        : custom { false }
    {
    }

    bool custom;

    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;

    int16_t gyro_x;
    int16_t gyro_y;
    int16_t gyro_z;
};

class Mpu {
public:
    Mpu(Calibration calibration = {})
        : m_calibration(calibration)
    {
    }

    bool initialize();
    bool update();

    int16_t yaw() const { return m_ypr[0]; }
    int16_t pitch() const { return m_ypr[1]; }
    int16_t roll() const { return m_ypr[2]; }

private:
    const int INTERRUPT_PIN { 13 };

    MPU6050 m_mpu {};
    Calibration m_calibration;

    bool m_dmp_ready { false };
    uint8_t m_device_status {};
    uint8_t m_interrupt_status {};
    uint16_t m_dmp_packet_size {};
    uint16_t m_dmp_fifo_size {};
    uint8_t m_dmp_buffer[64] {};

    Quaternion m_quaternion {};
    VectorFloat m_gravity {};
    std::array<float, 3> m_ypr_float {};
    std::array<int16_t, 3> m_ypr {};
    std::array<int16_t, 3> m_ypr_last {};

    static volatile bool m_interrupt;
    static void ICACHE_RAM_ATTR dmp_interrupt_handle() { Mpu::m_interrupt = true; }
};

volatile bool Mpu::m_interrupt { false };

bool Mpu::initialize()
{
    Wire.begin();
    Wire.setClock(400000);

    m_mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    Serial.println(F("Testing device connections..."));
    if (m_mpu.testConnection()) {
        Serial.println(F("MPU6050 connection successful"));
    } else {
        Serial.println(F("ERROR: MPU6050 connection failed"));
        return false;
    }

    Serial.println(F("Initializing DMP..."));
    m_device_status = m_mpu.dmpInitialize();

    m_mpu.setXAccelOffset(m_calibration.accel_x);
    m_mpu.setYAccelOffset(m_calibration.accel_y);
    m_mpu.setZAccelOffset(m_calibration.accel_z);

    m_mpu.setXGyroOffset(m_calibration.gyro_x);
    m_mpu.setYGyroOffset(m_calibration.gyro_y);
    m_mpu.setZGyroOffset(m_calibration.gyro_z);

    if (m_device_status == 0) {
        if (!m_calibration.custom) {
            Serial.println(F("Calibrating..."));
            m_mpu.CalibrateAccel(6);
            m_mpu.CalibrateGyro(6);
        } else {
            Serial.println(F("Calibration skipped"));
        }

        m_mpu.PrintActiveOffsets();

        Serial.println(F("Enabling DMP..."));
        m_mpu.setDMPEnabled(true);

        Serial.print(F("Enabling interrupt detection on external interrupt "));
        Serial.print(digitalPinToInterrupt(INTERRUPT_PIN));
        Serial.println(F("..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmp_interrupt_handle, RISING);
        m_interrupt_status = m_mpu.getIntStatus();

        Serial.println(F("DMP ready"));
        m_dmp_ready = true;
        m_dmp_packet_size = m_mpu.dmpGetFIFOPacketSize();
    } else {
        Serial.print(F("ERROR: DMP Initialization failed ("));
        if (m_device_status == 1) {
            Serial.print(F("initial memory load failed"));
        } else if (m_device_status == 2) {
            Serial.print(F("configuration updates failed"));
        } else {
            Serial.print(F("unknown"));
        }
        Serial.println(F(")"));

        return false;
    }

    return true;
}

bool Mpu::update()
{
    if (!m_dmp_ready || !m_interrupt) {
        return false;
    }

    while (m_dmp_fifo_size < m_dmp_packet_size) {
        m_dmp_fifo_size = m_mpu.getFIFOCount();
    }

    m_interrupt = false;
    m_interrupt_status = m_mpu.getIntStatus();
    m_dmp_fifo_size = m_mpu.getFIFOCount();

    if (m_dmp_fifo_size < m_dmp_packet_size) {
        Serial.println(F("ERROR: DMP FIFO size < DMP packet size"));
    } else if ((m_interrupt_status & _BV(MPU6050_INTERRUPT_FIFO_OFLOW_BIT))
        || m_dmp_fifo_size >= 1024) {
        m_mpu.resetFIFO();
        Serial.println(F("ERROR: DMP FIFO overflow!"));
    } else if (m_interrupt_status & _BV(MPU6050_INTERRUPT_DMP_INT_BIT)) {
        while (m_dmp_fifo_size >= m_dmp_packet_size) {
            m_mpu.getFIFOBytes(m_dmp_buffer, m_dmp_packet_size);
            m_dmp_fifo_size -= m_dmp_packet_size;
        }

        m_mpu.dmpGetQuaternion(&m_quaternion, m_dmp_buffer);
        m_mpu.dmpGetGravity(&m_gravity, &m_quaternion);
        m_mpu.dmpGetYawPitchRoll(m_ypr_float.data(), &m_quaternion, &m_gravity);

        m_ypr = {
            static_cast<int16_t>(m_ypr_float[0] * 180.0F / M_PI),
            static_cast<int16_t>(m_ypr_float[1] * 180.0F / M_PI),
            static_cast<int16_t>(m_ypr_float[2] * 180.0F / M_PI),
        };

        bool changed { m_ypr != m_ypr_last };
        m_ypr_last = m_ypr;

        return changed;
    }

    return false;
}