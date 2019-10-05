#include <ESP8266WiFi.h>

const char* ssid = "RaspberryConsole";
const char* password = "Korobeiniki1984";

IPAddress server(192, 168, 12, 1);
const int port = 1984;
WiFiClient client;

int timer = 0;

void connectWiFi();
void connectServer();

void setup()
{
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

    client.println(timer);
    timer += 1;

    delay(1000);
}
