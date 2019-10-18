#include "connection.hpp"

void Connection::connect()
{
    if (WiFi.status() != WL_CONNECTED) {
        connectWiFi();
    }

    if (!client.connected()) {
        connectServer();
    }
}

void Connection::connectWiFi()
{
    Serial.printf("Connecting to '%s' using password '%s' ", ssid, password);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("*");
    }

    Serial.printf("\r\nConnected to %s\r\n", ssid);
}

void Connection::connectServer()
{
    Serial.printf("Connecting to %s:%d ", ip, port);
    while (!client.connect(ip, port)) {
        delay(500);
        Serial.print(".");

        if (WiFi.status() != WL_CONNECTED) {
            Serial.println();
            connectWiFi();
        }
    }

    Serial.printf("\r\nConnected to %s\r\n", ip);
    client.setNoDelay(true);

    client.write(PROTOCOL_GREETING, sizeof(PROTOCOL_GREETING));
}

void Connection::sendButton(const Button& button)
{
    uint8_t message[] = {
        PROTOCOL_BUTTON_PREFIX,
        button.id,
        button.state == LOW ? PROTOCOL_BUTTON_DOWN : PROTOCOL_BUTTON_UP,
        PROTOCOL_DELIMITER,
    };

    client.write(message, sizeof(message));
}
