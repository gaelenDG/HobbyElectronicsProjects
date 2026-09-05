// ======== Library initialization ========
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <time.h>

// ======== Function prototypes ======== //

//  WiFi/MQTT functions ====
void connectToWiFi();
// const char* wifiStatusToString(wl_status_t status);
// void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info);

void connectToMQTT();
void mqttLog(const String& message);
void onMQTTMessage(char* topic, byte* payload, unsigned int length);
// bool syncTime(uint32_t timeoutMs);

// NeoPixel & LED strand functions ====
void lightPixel(int position, int Red, int Green, int Blue, int White);
void lightStrand(int PWM);

// Sensor reading functions ====
void readSensors();
float readBatVoltage();
void checkBatteryAndSleepIfLow();

// Light patterns defined in patterns.cpp ====
void updateCurrentPattern();

void DefaultPattern();
void initPattern1();
void updatePattern2();
void updatePattern3();
void initPattern4();
void updatePattern4();

// Helpers for patterns ====

// Selects a random color from a predefined list.
void pickRandomColor(int &r, int &g, int &b);

