// ======== Library initialization ========
#include <Arduino.h>
#include "config.h"
#include "helpers.h"

// ======== Setup ========
void setup() {
  // Start Serial connection
  Serial.begin(115200);
  delay(500);
  Serial.println("Booting...");

  // // Establish external wifi antenna connection
  // digitalWrite(3, LOW);//turn on this function
  // delay(100);
  // pinMode(14, OUTPUT);  
  // digitalWrite(14, HIGH);//use external antenna

  // Battery check - sleep if too low
  checkBatteryAndSleepIfLow();

  // Initialize NeoPixels light
  NeoPixelChain.begin();

  lightPixel(0, 10, 0, 0, 0); // Dim red light to show power on
  
  // NeoPixelChain.setPixelColor(0, NeoPixelChain.Color(10, 0, 0, 0)); // Dim red light to show power on
  // NeoPixelChain.show();

  // Connect to WiFi/MQTT
  connectToWiFi();
  connectToMQTT();

  lightPixel(0, 0, 0, 10, 0); 

  // NeoPixelChain.setPixelColor(0, NeoPixelChain.Color(0, 0, 10, 0)); // Dim blue light to show MQTT & Wifi connection
  // NeoPixelChain.show();

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


  lightPixel(0, 0, 0, 0, 0); // Light off to indicate setup is complete              

  // esp_sleep_enable_timer_wakeup(10000000); // Wake up every 10s to run sensor readings
}

void loop () {

  if(WiFi.status() != WL_CONNECTED) {
    logMsg = "WiFi disconnected. Reconnecting...";
    Serial.println(logMsg);

    connectToWiFi();
    mqttLog(logMsg);

    // get the current time on re-connect to WiFi
    // syncTime();
  }

  if(!mqttClient.connected()) {
    logMsg = "MQTT disconnected. Reconnecting...";
    Serial.println(logMsg);

    connectToMQTT();
    mqttLog(logMsg);
  }
  
  mqttClient.loop();          // Listen for MQTT updates & keep MQTT alive

  // Check peripherals
  readSensors();        // Temp/humidity/pressure

  updateCurrentPattern();      // animate LEDs

}