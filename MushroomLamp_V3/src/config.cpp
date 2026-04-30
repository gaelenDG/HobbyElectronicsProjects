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

uint8_t LDR = 3; // LDR GPIO

// ======== Voltage Reader & Battery control ========
uint8_t VoltageReader_Pin = 2;
float LOW_BAT_THRESHOLD = 3.4; // A limit point to trigger deep sleep if the battery is too low
uint64_t LOW_BAT_SLEEP_INTERVAL = 1; // Minutes between battery checks
int WAKE_BAT_THRESHOLD = 3.6; // threshold for sufficient battery voltage to wake back up
float NO_BAT_THRESHOLD = 2.0;


// ======== MQTT & WiFi ========

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// MQTT topics
const char* MQTT_TOPIC_Temp = "esp32/FairyGarden/Sensors/Temp";
const char* MQTT_TOPIC_Pressure = "esp32/FairyGarden/Sensors/Pressure";
const char* MQTT_TOPIC_Humidity = "esp32/FairyGarden/Sensors/Humidity";
const char* MQTT_TOPIC_Battery = "esp32/FairyGarden/Sensors/Battery";
const char* MQTT_TOPIC_Pattern = "esp32/FairyGarden/lights/set_pattern";
const char* MQTT_TOPIC_BRIGHTNESS = "esp32/FairyGarden/lights/set_brightness";
const char* MQTT_TOPIC_SUNSET = "esp32/FairyGarden/SunSet";
const char* MQTT_TOPIC_SUNRISE = "esp32/FairyGarden/SunRise";

String logMsg;

// ======== Sensor timing ========

unsigned long lastSensorCheck = 60000UL;
unsigned long SensorCheck_Interval = 60000UL;
unsigned long lastWiFicheck = lastSensorCheck;
unsigned long lastMQTTcheck = lastSensorCheck;
unsigned long lastBattCheck = lastSensorCheck;

// ======== NeoPixel & Sensor definitions ========

// uint8_t numPixels = 8;  // Total pixels
int patternIndex = 0;
int previousPattern = 0; 
int maxBrightness = 100;

Adafruit_NeoPixel NeoPixelChain(NUM_PIXELS, NeoPixelChain_gpio, NEO_GRBW + NEO_KHZ800);

Adafruit_AHTX0 aht; // AHT20 temperature sensor

Adafruit_BMP280 bmp; // BMP280 pressure sensor

// ============ pattern vars ============

// Color options for pattern 4
const int colorOptions[8][3] = {
  {255, 0, 0},    // Red
  {0, 255, 0},    // Green
  {0, 0, 255},    // Blue
  {255, 255, 0},  // Yellow
  {0, 255, 255},  // Cyan
  {255, 0, 255},  // Magenta
  {255, 165, 0},  // Orange
  {75, 0, 130}    // Indigo
};

const int numColors = sizeof(colorOptions) / sizeof(colorOptions[0]); // helper var for pattern4

float brightness_arr[NUM_PIXELS];
unsigned long startTime_arr[NUM_PIXELS];
int cycleDuration_arr[NUM_PIXELS];
int colors_arr[NUM_PIXELS][3];
int newColors_arr[NUM_PIXELS][3];
bool waitingForMin_arr[NUM_PIXELS];
bool wasDecreasing_arr[NUM_PIXELS];