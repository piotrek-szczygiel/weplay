#pragma once

#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define MAX_UDP_PACKET_SIZE 1500

class Connection {
private:
    WiFiClient m_client;
    const char* m_ssid;
    const char* m_password;
    uint16_t m_port;

    WiFiUDP m_udp;
    char m_udp_packet[MAX_UDP_PACKET_SIZE];
    IPAddress m_ip;
    bool m_discovered { false };

    void connect_wifi();
    void discover_console_ip();
    void connect_server();

public:
    Connection(const char* ssid, const char* password, uint16_t port)
        : m_ssid(ssid)
        , m_password(password)
        , m_port(port)
    {
    }

    void connect();
    void ping();

    void send_buttons(uint16_t buttons_state);
    void send_ypr(int16_t yaw, int16_t pitch, int16_t roll);
};
