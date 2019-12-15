#pragma once

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define MAX_UDP_PACKET_SIZE 1500

class Connection {
public:
    Connection(const char* ssid, const char* password, uint16_t port)
        : m_ssid(ssid)
        , m_password(password)
        , m_port(port)
    {
    }

    void connect();
    void ping();
    void pong();

    void update(uint16_t buttons_state, int16_t yaw, int16_t pitch, int16_t roll);

private:
    const char* m_ssid;
    const char* m_password;

    WiFiUDP m_udp;

    IPAddress m_ip;
    uint16_t m_port;

    char m_udp_packet[MAX_UDP_PACKET_SIZE];

    bool m_discovered { false };
    bool m_connected { false };

    int m_ping { 0 };

    void connect_wifi();
    void discover_console_ip();
    void connect_server();
};
