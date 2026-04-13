// ======== Library initialization ========
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

// ======== Function prototypes ======== //

// Core functions ====
float readBatVoltage();
void checkBatteryAndSleepIfLow();

//  WiFi/MQTT functions ====
void connectToWiFi();
void connectToMQTT();
void mqttLog(const String& message);
void onMQTTMessage(char* topic, byte* payload, unsigned int length);

// NeoPixel functions ====
void lightPixel(int position, int Red, int Green, int Blue, int White);

// Weather functions ====
void readWeatherSensors();

// Light patterns Defined in patterns.cpp
void updateCurrentPattern();
void updateDefaultPattern();
void updatePattern1();
void updatePattern2();
void updatePattern3();
void updatePattern4();
