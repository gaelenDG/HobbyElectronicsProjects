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
      StatusLight.setPixelColor(0, StatusLight.Color(1, 0, 0)); // Dim green light for booting feedback
      StatusLight.show();
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