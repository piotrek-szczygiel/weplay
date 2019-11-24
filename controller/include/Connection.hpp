#pragma once

#include <ESP8266WiFi.h>

class Connection {
private:
    const char* m_ssid;
    const char* m_password;
    const char* m_ip;
    uint16_t m_port;

    WiFiClient m_client;

    void connect_wifi();
    void connect_server();

public:
    Connection(const char* ssid, const char* password, const char* ip, uint16_t port)
        : m_ssid(ssid)
        , m_password(password)
        , m_ip(ip)
        , m_port(port)
    {
    }

    void connect();

    void send_buttons(uint16_t buttons_state);
    void send_ypr(int16_t yaw, int16_t pitch, int16_t roll);
};
