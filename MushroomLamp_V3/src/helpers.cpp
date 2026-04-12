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
      mqttClient.subscribe(MQTT_TOPIC_Temp);
      mqttClient.subscribe(MQTT_TOPIC_Pressure);
      mqttClient.subscribe(MQTT_TOPIC_Humidity);
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

// The whole sequence to taking the temp/pressure/humidity readings
void readWeatherSensors() {

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

  mqttClient.publish(MQTT_TOPIC_Temp, String(temp.temperature, 1).c_str());
  mqttClient.publish(MQTT_TOPIC_Pressure, String(bmp.readPressure() / 100.0F, 1).c_str());
  mqttClient.publish(MQTT_TOPIC_Humidity, String(humidity.relative_humidity, 1).c_str());

  
  // // cascading signal that we took a measurement
  // for (int i = 0; i < numPixels; i++) {
  //   lightPixel(i, 10, 0, 0, 0); // dim red light
  //   delay(250);
  //   lightPixel(i, 0, 0, 0, 0); // shut off
  // }
  lightPixel(0, 5, 0, 0, 0); // dim red light
  delay(250);
  lightPixel(0, 0, 0, 0, 0); // shut off
}

void lightPixel(int position, int Red, int Green, int Blue, int White) {

  // Determine which chain and pixel to light up
  Adafruit_NeoPixel* chain = nullptr; // Pointer to the correct chain, once determined
  uint32_t color = 0;  // Store the calculated color

  chain = &NeoPixelChain;

  color = chain->Color(Red, Green, Blue, White); // RGB pixel color assignment

  chain->setPixelColor(position, color);
  // chain->setBrightness(LEDbrightness);

  chain->show();
  
}