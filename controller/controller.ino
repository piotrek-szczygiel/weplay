#include <ESP8266WiFi.h>

void connectWiFi();
void connectServer();

const char* ssid = "esp8266";
const char* password = "matebook";

IPAddress server(192, 168, 137, 1);
const int port = 1984;

WiFiClient client;

const int buttonsCount = 3;
int lastState[buttonsCount] = { HIGH, HIGH, HIGH };
int buttonsPin[buttonsCount] = { 14, 12, 13 };

void setup()
{
    for (int i = 0; i < buttonsCount; ++i) {
        pinMode(buttonsPin[i], INPUT_PULLUP);
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

    char message[buttonsCount + 1];
    bool sendMessage = false;

    for (int i = 0; i < buttonsCount; ++i) {
        int state = digitalRead(buttonsPin[i]);

        if (state != lastState[i]) {
            sendMessage = true;
        }

        lastState[i] = state;
        message[i] = (state == LOW ? 'x' : '.');
    }

    if (sendMessage) {
        message[buttonsCount] = '\0';
        client.println(message);
        Serial.println(message);
    }
}
