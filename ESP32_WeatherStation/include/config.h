#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <PubSubClient.h>

// ======== NeoPixel Declarations ========
extern Adafruit_NeoPixel StatusLight;

// ======== GPIO & I2C Declarations ========
extern const int I2C_SDA_PIN;  // GPIO pin for I2C SDA
extern const int I2C_SCL_PIN;  // GPIO pin for I2C SCL

extern const int LDR_up; 
extern const int LDR_down;
extern const int LDR_left;
extern const int LDR_right;

extern const int BuiltIn_NeoPixel;

// ======== MQTT Topics ========

extern const char* MQTT_TOPIC_Temp;
extern const char* MQTT_TOPIC_Pressure;
extern const char* MQTT_TOPIC_Humidity;

extern String logMsg;

extern WiFiClient espClient;
extern PubSubClient mqttClient;

// ======== Sensor Declarations ========
extern Adafruit_AHTX0 aht; // AHT20 temperature sensor
 
extern Adafruit_BMP280 bmp; // BMP280 pressure sensor


#endif // CONFIG_H
