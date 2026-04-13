// ======== Library initialization ========
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include "config.h"
#include "helpers.h"

using namespace std;

// ======== GPIO & I2C Declarations ========

uint8_t NeoPixelChain_gpio = 21; // NeoPixel Chain GPIO

uint8_t I2C_SDA_PIN = 22;  // GPIO pin for I2C SDA
uint8_t I2C_SCL_PIN = 23;  // GPIO pin for I2C SCL

uint8_t LDR = 3; // LDR Array GPIOs

// ======== Voltage Reader & Battery control ========
uint8_t VoltageReader_Pin = 2;
unsigned long lastBattCheck = 10000UL;
uint64_t Batt_Check_Interval = 10000UL;
float LOW_BAT_THRESHOLD = 3.4; // A limit point to trigger deep sleep if the battery is too low
uint64_t LOW_BAT_SLEEP_INTERVAL = 1; // Minutes between battery checks
int WAKE_BAT_THRESHOLD = 3.6; // threshold for sufficient battery voltage to wake back up
float NO_BAT_THRESHOLD = 2.0;

unsigned long lastSensorCheck = 10000UL;

// ======== MQTT & WiFi ========

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// MQTT topics
const char* MQTT_TOPIC_Temp = "esp32/WeatherStation/Temp";
const char* MQTT_TOPIC_Pressure = "esp32/WeatherStation/Pressure";
const char* MQTT_TOPIC_Humidity = "esp32/WeatherStation/Humidity";
const char* MQTT_TOPIC_Battery = "esp32/WeatherStation/Battery";
const char* MQTT_TOPIC_Pattern = "lamp/pattern/set";

String logMsg;

unsigned long lastWiFicheck = 10000UL;
unsigned long lastMQTTcheck = 10000UL;

// ======== Status light & Sensor definitions ========

uint8_t numPixels = 8;  // Total pixels
int patternIndex = 0;

Adafruit_NeoPixel NeoPixelChain(numPixels, NeoPixelChain_gpio, NEO_GRBW + NEO_KHZ800);
// Adafruit_NeoPixel NeoPixel_Chain(64, 2, NEO_GRB + NEO_KHZ800);

Adafruit_AHTX0 aht; // AHT20 temperature sensor
 
Adafruit_BMP280 bmp; // BMP280 pressure sensor


