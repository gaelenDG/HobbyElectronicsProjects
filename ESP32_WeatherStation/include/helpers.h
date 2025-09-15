// ======== Library initialization ========
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

// ======== Function prototypes ======== //

// All defined in helpers.cpp

void connectToWiFi();
void connectToMQTT();
void mqttLog(const String& message);
int checkAlignmentUp();
int checkAlignmentLateral();