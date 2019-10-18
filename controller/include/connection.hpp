#pragma once

#include "button.hpp"
#include <ESP8266WiFi.h>

const uint8_t PROTOCOL_DELIMITER = '\n';

const uint8_t PROTOCOL_GREETING[] = {
    0x19, // Magic byte
    0x84, // Magic byte
    0x01, // Protocol version
    PROTOCOL_DELIMITER,
};

const uint8_t PROTOCOL_BUTTON_PREFIX = 'b';
const uint8_t PROTOCOL_BUTTON_DOWN = 'd';
const uint8_t PROTOCOL_BUTTON_UP = 'u';

class Connection {
private:
    const char* ssid;
    const char* password;
    const char* ip;
    uint16_t port;

    WiFiClient client;

    void connectWiFi();
    void connectServer();

public:
    Connection(const char* ssid, const char* password, const char* ip, uint16_t port)
        : ssid(ssid)
        , password(password)
        , ip(ip)
        , port(port)
    {
    }

    void connect();
    void sendButton(const Button& button);
};
