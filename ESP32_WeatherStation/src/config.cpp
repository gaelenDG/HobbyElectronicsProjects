// ======== Library initialization ========
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include "config.h"

using namespace std;

// ======== GPIO & I2C Declarations ========
const int I2C_SDA_PIN = 1;  // GPIO pin for I2C SDA
const int I2C_SCL_PIN = 2;  // GPIO pin for I2C SCL

// LDR Array GPIOs
const int LDR_up = 3; 
const int LDR_down = 4;
const int LDR_left = 5;
const int LDR_right = 6;

// Status light GPIO
const int BuiltIn_NeoPixel = 21;

// ======== MQTT Topics ========

const char* MQTT_TOPIC_Temp = "esp32/WeatherStation/Temp";
const char* MQTT_TOPIC_Pressure = "esp32/WeatherStation/Pressure";
const char* MQTT_TOPIC_Humidity = "esp32/WeatherStation/Humidity";

String logMsg;

// ======== Status light & Sensor definitions ========

Adafruit_NeoPixel StatusLight(1, BuiltIn_NeoPixel, NEO_GRB + NEO_KHZ800);

Adafruit_AHTX0 aht; // AHT20 temperature sensor
 
Adafruit_BMP280 bmp; // BMP280 pressure sensor

// ======== WiFi & MQTT client info ========

WiFiClient espClient;
PubSubClient mqttClient(espClient);

