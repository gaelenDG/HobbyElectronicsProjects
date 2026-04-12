// ======== Library initialization ========
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

// ======== Function prototypes ======== //

//  WiFi/MQTT functions ====
void connectToWiFi();
void connectToMQTT();
void mqttLog(const String& message);

// NeoPixel functions ====

void lightPixel(int position, int Red, int Green, int Blue, int White);

// Weather functions ====

void readWeatherSensors();