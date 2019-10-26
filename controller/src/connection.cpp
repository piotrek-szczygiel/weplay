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
    Serial.print(F("Connecting to "));
    Serial.println(m_ssid);

    WiFi.begin(m_ssid, m_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(F("*"));
    }

    Serial.print(F("\r\nConnected to "));
    Serial.println(m_ssid);
}

void Connection::connect_server()
{
    Serial.print(F("Connecting to "));
    Serial.print(m_ip);
    Serial.print(":");
    Serial.println(m_port);
    while (!m_client.connect(m_ip, m_port)) {
        delay(500);
        Serial.print(F("."));

        if (WiFi.status() != WL_CONNECTED) {
            Serial.println();
            connect_wifi();
        }
    }

    Serial.print(F("Connected to "));
    Serial.println(m_ip);
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
