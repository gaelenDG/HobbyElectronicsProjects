#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <WiFi.h>
#include <PubSubClient.h>

// ======== NeoPixel Declarations ========
extern Adafruit_NeoPixel NeoPixelChain;

extern uint8_t numPixels;  // Total pixels
extern int maxBrightness; // Maximum brightness of patterns

extern int patternIndex; // Starting pattern = "0"


// ======== GPIO & I2C Declarations ========
extern uint8_t I2C_SDA_PIN;  // GPIO pin for I2C SDA
extern uint8_t I2C_SCL_PIN;  // GPIO pin for I2C SCL
extern uint8_t VoltageReader_Pin; // GPIO pin for battery voltage reader
extern uint8_t LDR; 

// ======== MQTT Topics ========

extern const char* MQTT_TOPIC_Temp;
extern const char* MQTT_TOPIC_Pressure;
extern const char* MQTT_TOPIC_Humidity;
extern const char* MQTT_TOPIC_Battery;
extern const char* MQTT_TOPIC_Pattern;

extern String logMsg;

extern WiFiClient espClient;
extern PubSubClient mqttClient;

// ======== Sensor Declarations ========
extern Adafruit_AHTX0 aht; // AHT20 temperature sensor
 
extern Adafruit_BMP280 bmp; // BMP280 pressure sensor

// Battery checking timing and thresholds
extern float NO_BAT_THRESHOLD;
extern float LOW_BAT_THRESHOLD; // A limit point to trigger deep sleep if the battery is too low
extern uint64_t Batt_Check_Interval;
extern uint64_t LOW_BAT_SLEEP_INTERVAL; // Minutes between battery checks while charging
extern int WAKE_BAT_THRESHOLD; 

extern unsigned long lastBattCheck;
extern unsigned long lastSensorCheck;
extern unsigned long lastWiFicheck;
extern unsigned long lastMQTTcheck;

#endif // CONFIG_H
