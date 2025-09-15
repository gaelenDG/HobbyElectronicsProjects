// ======== Library initialization ========

#include <Arduino.h>
#include "config.h"
#include "helpers.h"
using namespace std;

// ======== Setup ========

void setup() {

  // Initialize status light
  StatusLight.begin();
  StatusLight.setPixelColor(0, StatusLight.Color(10, 0, 0)); // Dim red light to show power on
  StatusLight.show();

  // Start Serial connection
  Serial.begin(115200);
  delay(500);
  Serial.println("Booting...");

  // Connect to HA
  connectToWiFi();
  connectToMQTT();

  StatusLight.setPixelColor(0, StatusLight.Color(0, 0, 10)); // Dim blue light to show MQTT & Wifi connection
  StatusLight.show();

  // Start I2C
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  // Start AHT temp sensor
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }

  // Start BMP280 pressure/humidity sensor
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }
  Serial.println("AHT20 & BMP280 found!");

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */


  StatusLight.setPixelColor(0, StatusLight.Color(0, 0, 0)); // Light off to indicate setup is complete
  StatusLight.show();

  esp_sleep_enable_timer_wakeup(30000000); // Wake up every 10s to run sensor readings
}

void loop () {
  Serial.println("Waking up!");


  // Check if WiFi and MQTT are connected
  if (WiFi.status() != WL_CONNECTED) {
    logMsg = "WiFi disconnected. Reconnecting...";
    Serial.println(logMsg);
    

    connectToWiFi();
    mqttLog(logMsg);
  }
  if (!mqttClient.connected()) {
    logMsg = "MQTT disconnected. Reconnecting...";
    Serial.println(logMsg);
    
    connectToMQTT();
    mqttLog(logMsg);
  }
  
  StatusLight.setPixelColor(0, StatusLight.Color(10, 0, 0)); // Brief red pulse to show sensors are being read
  StatusLight.show();

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

  // set cursor to first column, first row
  // lcd.setCursor(0, 0);
  // // print static message
  // lcd.print(messageStatic);

  // // print scrolling message
  // scrollText(1, messageToScroll, 250, lcdColumns);

  mqttClient.publish(MQTT_TOPIC_Temp, String(temp.temperature, 1).c_str());
  mqttClient.publish(MQTT_TOPIC_Pressure, String(bmp.readPressure() / 100.0F, 1).c_str());
  mqttClient.publish(MQTT_TOPIC_Humidity, String(humidity.relative_humidity, 1).c_str());



  delay(500);

  StatusLight.setPixelColor(0, StatusLight.Color(0, 0, 0)); // Turn off light to show the device is asleep.
  StatusLight.show();

  delay(500);

  // delay(10000);

  Serial.println("Going to sleep");
  
  esp_light_sleep_start(); // Enter light sleep
}