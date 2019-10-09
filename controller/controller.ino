#include <ESP8266WiFi.h>

void connectWiFi();
void connectServer();

const char* ssid = "RaspberryConsole";
const char* password = "Korobeiniki1984";

IPAddress server(192, 168, 12, 1);
const int port = 1984;

WiFiClient client;

#define MESSAGE_LENGTH 16
char message[MESSAGE_LENGTH];

#define BUTTON_COUNT 3
const int buttonPin[BUTTON_COUNT] = { 14, 12, 13 };
const char* buttonNames[BUTTON_COUNT] = { "left", "middle", "right" };
int lastButtonState[BUTTON_COUNT] = { HIGH, HIGH, HIGH };

void setup()
{
    for (int i = 0; i < BUTTON_COUNT; ++i) {
        pinMode(buttonPin[i], INPUT_PULLUP);
    }

    Serial.begin(115200);
    delay(100);
    Serial.println();
    Serial.println("Program started");
    connectWiFi();
    connectServer();
}

void connectWiFi()
{
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("*");
    }

    Serial.println();
    Serial.println("WiFi connected");
}

void connectServer()
{
    Serial.println("Connecting to server");
    while (!client.connect(server, port)) {
        delay(500);
        Serial.print("*");

        if (WiFi.status() != WL_CONNECTED) {
            Serial.println();
            connectWiFi();
        }
    }

    Serial.println();
    Serial.println("Connected to server");

    client.setNoDelay(true);
}

void loop()
{
    if (!client.connected()) {
        connectServer();
    }

    for (int i = 0; i < BUTTON_COUNT; ++i) {
        int state = digitalRead(buttonPin[i]);

        if (state != lastButtonState[i]) {
            snprintf(message, MESSAGE_LENGTH, "%s %s\r\n", buttonNames[i], state == LOW ? "down" : "up");
            client.write(message);
            Serial.print(message);
        }

        lastButtonState[i] = state;
    }
}
