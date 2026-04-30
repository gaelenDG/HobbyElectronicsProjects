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
#define NUM_PIXELS 8 // Total pixels
extern int maxBrightness; // Maximum brightness of patterns

extern int patternIndex; // Starting pattern = "0"
extern int previousPattern; // tracks the previously defined pattern and forces alignment if needed.


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
extern const char* MQTT_TOPIC_BRIGHTNESS;
extern const char* MQTT_TOPIC_SUNSET;
extern const char* MQTT_TOPIC_SUNRISE;

extern String logMsg;

extern WiFiClient espClient;
extern PubSubClient mqttClient;

// ======== Sensor Declarations ========
extern Adafruit_AHTX0 aht; // AHT20 temperature sensor
 
extern Adafruit_BMP280 bmp; // BMP280 pressure sensor

// Battery checking timing and thresholds
extern float NO_BAT_THRESHOLD;
extern float LOW_BAT_THRESHOLD; // A limit point to trigger deep sleep if the battery is too low
extern uint64_t LOW_BAT_SLEEP_INTERVAL; // Minutes between battery checks while charging
extern int WAKE_BAT_THRESHOLD; 

extern unsigned long lastSensorCheck;
extern unsigned long SensorCheck_Interval;

extern unsigned long lastBattCheck;
extern unsigned long lastSensorCheck;
extern unsigned long lastWiFicheck;
extern unsigned long lastMQTTcheck;

// ============ pattern vars ============
extern const int numColors; // helper var for pattern4
extern const int colorOptions[8][3]; // Explicitly declare color options for pattern 4 as a 2D array

// Pattern 3

// vector<int> graph[8] = { // A (very simple) map of the NeoPixels for adjacency
//   {1},
//   {0, 2},
//   {1, 3},
//   {2, 4},
//   {3, 5},
//   {4, 6},
//   {5, 7},
//   {6, 0}
// };

// Pattern 4
extern float brightness_arr[NUM_PIXELS];
extern unsigned long startTime_arr[NUM_PIXELS];
extern int cycleDuration_arr[NUM_PIXELS];
extern int colors_arr[NUM_PIXELS][3];
extern int newColors_arr[NUM_PIXELS][3];
extern bool wasDecreasing_arr[NUM_PIXELS];
extern bool waitingForMin_arr[NUM_PIXELS];

#endif // CONFIG_H
