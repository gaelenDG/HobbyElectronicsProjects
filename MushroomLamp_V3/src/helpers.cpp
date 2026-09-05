// ======== Library initialization ========
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_NeoPixel.h>
#include "helpers.h"
#include "config.h"
#include "secrets.h"

using namespace std;

// Battery management functions ====
float readBatVoltage() {
  uint32_t Vbatt = 0;
  for(int i = 0; i < 16; i++) {
    Vbatt += analogReadMilliVolts(VoltageReader_Pin); // Read and accumulate ADC voltage
    delay(2);
  }
  float Vbattf = 2 * Vbatt / 16 / 1000.0; 
  
  return Vbattf;
}

void checkBatteryAndSleepIfLow() {
  
  float Vbat = readBatVoltage();
  Serial.printf("Battery voltage: %.2f V\n", Vbat);
  mqttClient.publish(MQTT_TOPIC_Battery, String(Vbat, 1).c_str());
  
  // Case 1: No battery connected
  if (Vbat < NO_BAT_THRESHOLD) {
    Serial.println("⚠️ No battery detected — continuing normal operation.");
    return;
  }
  
  // Case 2: Battery low
  if (Vbat < LOW_BAT_THRESHOLD) {
    Serial.println("Low battery! Charge me!");
    
    // Compute sleep duration in microseconds
    uint64_t sleep_us = (uint64_t)LOW_BAT_SLEEP_INTERVAL * 120ULL * 1000000ULL;
    
    Serial.printf("Sleeping for %d minutes before rechecking battery.\n",
      LOW_BAT_SLEEP_INTERVAL);
      
      // Configure wake timer
      esp_sleep_enable_timer_wakeup(sleep_us);
      esp_deep_sleep_start();
    }
    
    // Case 3: Battery OK — continue running
    Serial.println("Battery OK — continuing normal operation.");
  }
  
  //  WiFi/MQTT functions ====
  void connectToWiFi() {
    logMsg = "Connecting to Wi-Fi";
    Serial.print(logMsg);
    mqttLog(logMsg);
    
    WiFi.begin(ssid, password);
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
      delay(500);
      Serial.print(".");
      attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWi-Fi connected");
      mqttLog("Wi-Fi connected");
    } else {
      Serial.println("\nWi-Fi connection failed after 10 attempts.");
      mqttLog("Wi-Fi connection failed after 10 attempts.");
    }
  }
  
  void connectToMQTT() {
    mqttClient.setServer(mqttServer, mqttPort);
    logMsg = "Connecting to MQTT broker...";
    Serial.print(logMsg);
    mqttLog(logMsg);
    
    int attempts = 0;
    while (!mqttClient.connected() && attempts < 10) {
      if (mqttClient.connect("ESP32-S3-Zero WeatherStation", mqttUser, mqttPassword)) {
        mqttClient.setCallback(onMQTTMessage);
        mqttClient.subscribe(MQTT_TOPIC_Temp);
        mqttClient.subscribe(MQTT_TOPIC_Pressure);
        mqttClient.subscribe(MQTT_TOPIC_Humidity);
        mqttClient.subscribe(MQTT_TOPIC_Pattern);
        mqttClient.subscribe(MQTT_TOPIC_BRIGHTNESS);
        Serial.println("connected"); // Report MQTT connection status
        mqttLog("MQTT connected");
        break;
      } else {
        Serial.print(".");
        delay(500);
        attempts++;
      }
    }
    if (!mqttClient.connected()) {
      Serial.println("\nMQTT connection failed after 10 attempts.");
      mqttLog("MQTT connection failed after 10 attempts.");
    }
  }
  
  void mqttLog(const String& message) {
    mqttClient.publish("esp32/WeatherStation/Log", message.c_str());
  }
  
  void onMQTTMessage(char* topic, uint8_t* payload, unsigned int length) {
    char msg[length + 1];
    memcpy(msg, payload, length);
    msg[length] = '\0';  // null terminate
    
    Serial.print("Topic: ");
    Serial.println(topic);
    
    Serial.print("Payload: ");
    Serial.println(msg);
    
    // ===== Pattern control =====
    if (strcmp(topic, MQTT_TOPIC_Pattern) == 0) {
      int newPattern = atoi(msg);
      
      if (newPattern >= 0 && newPattern <= 4) {
        DefaultPattern();
        patternIndex = newPattern;
        Serial.printf("Switched to pattern %d\n", patternIndex);
        mqttClient.publish("esp32/FairyGarden/lights/pattern_state", String(patternIndex).c_str()); // Publish new state to HA
      }
    }
    
    // ===== Brightness control =====
    else if (strcmp(topic, MQTT_TOPIC_BRIGHTNESS) == 0) {
      int newBrightness = atoi(msg);
      
      if (newBrightness >= 0 && newBrightness <= 100) {
        if (strlen(msg) > 0 && isdigit(msg[0])) {
          newBrightness = constrain(newBrightness, 0, 100); // Clamp to valid range
          maxBrightness = newBrightness;
          
          mqttClient.publish("esp32/FairyGarden/lights/current_brightness", String(maxBrightness).c_str()); // Publish new state to HA
          
          Serial.printf("Brightness set to %d%%\n", maxBrightness);
        }
      }
    }
  }
  
  // Sensor functions ====
  
  // The whole sequence to taking the temp/pressure/humidity readings
  void readSensors() {
    
    // Skip sensor reads if it hasn't been very long since last check (same interval as battery check)
    unsigned long Sensor_now = millis();
    
    
    if (Sensor_now - lastSensorCheck < SensorCheck_Interval) return;
    // Update last check timestamp immediately
    lastSensorCheck = Sensor_now;
    
    checkBatteryAndSleepIfLow(); // Battery check - sleep if too low   
    
    if (aht.begin() & bmp.begin()) {
      
      sensors_event_t humidity, temp;
      aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
      
      // must call this to wake sensor up and get new measurement data
      // it blocks until measurement is complete
      if (bmp.takeForcedMeasurement()) {
        
        // Read data from BMP280
        float pressure = bmp.readPressure() / 100.0F; // Convert to hPa
        float altitude = bmp.readAltitude(1013.25); // Sea level pressure in hPa
        
      } else {
        Serial.println("Forced measurement failed!");
      }
      
      Serial.println( "Temp = " + String(temp.temperature, 1) + " C" + 
      " | Pressure = " + String(bmp.readPressure() / 100.0F, 1) + " Pa" +
      " | Humidity = " + String(humidity.relative_humidity, 1) + " %"
    );
    
    // int LDR_reading = analogReadMilliVolts(LDR_pin); // Read LDR
    // int Thermistor_reading = analogReadMilliVolts(thermistor_pin); // Read thermistor value
    
    mqttClient.publish(MQTT_TOPIC_Temp, String(temp.temperature, 1).c_str());
    mqttClient.publish(MQTT_TOPIC_Pressure, String(bmp.readPressure() / 100.0F, 1).c_str());
    mqttClient.publish(MQTT_TOPIC_Humidity, String(humidity.relative_humidity, 1).c_str());
  }
  
  
  // mqttClient.publish(MQTT_TOPIC_LDR, String(LDR_reading).c_str());
  
  // lightPixel(0, 0, 5, 0, 0); // dim green light
  // delay(250);
  // lightPixel(0, 0, 0, 0, 0); // shut off
}

// Neopixel & LED strand Functions ====
void lightPixel(int position, int Red, int Green, int Blue, int White) {
  
  // Determine which chain and pixel to light up
  Adafruit_NeoPixel* chain = nullptr; // Pointer to the correct chain, once determined
  uint32_t color = 0;  // Store the calculated color
  
  chain = &NeoPixelChain;
  
  color = chain->Color(Red, Green, Blue, White); // RGB pixel color assignment
  
  chain->setPixelColor(position, color);                    // Set the color of the designated pixel
  chain->setBrightness(map(maxBrightness, 0, 100, 0, 255)); // Scale brightness appropriately
  
  chain->show();
  
}

/*
* Sets the brightness of the LED strand using PWM.
* @param PWM A value between 0 (off) and 255 (full brightness)
*/
void lightStrand(int PWM) {
  // ledcWrite(LEDC_CHANNEL, PWM);  // Apply PWM value to LED strand
  // analogWrite(PWM_LED_PIN, PWM);
  ledcWrite(PWM_LED_PIN, PWM);
}
