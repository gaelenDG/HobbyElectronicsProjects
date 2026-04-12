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


const int NeoPixelChain_gpio = 21; // NeoPixel Chain GPIO

const int I2C_SDA_PIN = 22;  // GPIO pin for I2C SDA
const int I2C_SCL_PIN = 23;  // GPIO pin for I2C SCL

// LDR Array GPIOs
const int LDR = 3; 




// ======== MQTT Topics ========

const char* MQTT_TOPIC_Temp = "esp32/WeatherStation/Temp";
const char* MQTT_TOPIC_Pressure = "esp32/WeatherStation/Pressure";
const char* MQTT_TOPIC_Humidity = "esp32/WeatherStation/Humidity";

String logMsg;

// ======== Status light & Sensor definitions ========

const int numPixels = 8;  // Total pixels

Adafruit_NeoPixel NeoPixelChain(numPixels, NeoPixelChain_gpio, NEO_GRBW + NEO_KHZ800);
// Adafruit_NeoPixel NeoPixel_Chain(64, 2, NEO_GRB + NEO_KHZ800);

Adafruit_AHTX0 aht; // AHT20 temperature sensor
 
Adafruit_BMP280 bmp; // BMP280 pressure sensor

WiFiClient espClient;
PubSubClient mqttClient(espClient);
