#include "Connection.hpp"

void Connection::connect()
{
    if (WiFi.status() != WL_CONNECTED) {
        connect_wifi();
    }

    if (!m_client.connected()) {
        connect_server();
    }
}

void Connection::connect_wifi()
{
    Serial.printf("Connecting to '%s' using password '%s' ", m_ssid, m_password);
    WiFi.begin(m_ssid, m_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("*");
    }

    Serial.printf("\r\nConnected to %s\r\n", m_ssid);
}

void Connection::connect_server()
{
    Serial.printf("Connecting to %s:%d ", m_ip, m_port);
    while (!m_client.connect(m_ip, m_port)) {
        delay(500);
        Serial.print(".");

        if (WiFi.status() != WL_CONNECTED) {
            Serial.println();
            connect_wifi();
        }
    }

    Serial.printf("\r\nConnected to %s\r\n", m_ip);
    m_client.setNoDelay(true);
}

void Connection::send_ypr(int16_t yaw, int16_t pitch, int16_t roll)
{
    uint8_t packet[] {
        'M',
        static_cast<uint8_t>((yaw & 0x00ff)),
        static_cast<uint8_t>((yaw & 0xff00) >> 8),
        static_cast<uint8_t>((pitch & 0x00ff)),
        static_cast<uint8_t>((pitch & 0xff00) >> 8),
        static_cast<uint8_t>((roll & 0xff00)),
        static_cast<uint8_t>((roll & 0x00ff) >> 8),
    };

    m_client.write(packet, sizeof(packet));
}
