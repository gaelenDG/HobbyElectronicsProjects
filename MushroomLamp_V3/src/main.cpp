// ======== Library initialization ========
#include <Arduino.h>
#include "config.h"
#include "helpers.h"
#include <Adafruit_NeoPixel.h> 
// ======== Setup ========
void setup() {
  // Start Serial connection
  Serial.begin(115200);
  delay(500);
  Serial.println("Booting...");
  
  // // Establish external wifi antenna connection
  digitalWrite(3, LOW);//turn on this function
  delay(100);
  pinMode(14, OUTPUT);  
  digitalWrite(14, HIGH);//use external antenna
  
  // Battery check - sleep if too low
  checkBatteryAndSleepIfLow();
  
  // Initialize NeoPixels light
  NeoPixelChain.begin();
  
  lightPixel(0, 10, 0, 0, 0); // Dim red light to show power on
  
  // // Initialize LED strand with PWM settings
  ledcAttach(PWM_LED_PIN, 5000, 8);
  
  // Connect to WiFi/MQTT
  // WiFi.onEvent(WiFiEvent);
  connectToWiFi();
  connectToMQTT();
  
  lightPixel(0, 0, 0, 10, 0); 
  
  // Start I2C
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  
  Serial.println("Scanning I2C...");
  
  // for (byte address = 1; address < 127; address++) {
  //   Wire.beginTransmission(address);
  //   if (Wire.endTransmission() == 0) {
  //     Serial.printf("Found device at 0x%02X\n", address);
  //   }
  // }
  // Start AHT temp sensor
  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    for (int aht_connect = 1; aht_connect <= 10; aht_connect++)  {
      delay(10);
    }
  }
  
  // Start BMP280 pressure/humidity sensor
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
      "try a different address!"));
      for (int bmp_connect = 1; bmp_connect <= 10; bmp_connect++)  {
        delay(10);
      }
    }
    
    if (aht.begin() & bmp.begin()) {
      Serial.println("AHT20 & BMP280 found!");
      
      /* Default settings from datasheet. */
      bmp.setSampling(Adafruit_BMP280::MODE_FORCED,     /* Operating Mode. */
        Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
        Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
        Adafruit_BMP280::FILTER_X16,      /* Filtering. */
        Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
        
      } else {
        Serial.println("AHT20 & BMP280 *NOT* found!");
      }
      
      
      lightPixel(0, 0, 0, 0, 0); // Light off to indicate setup is complete              
      
      // esp_sleep_enable_timer_wakeup(10000000); // Wake up every 10s to run sensor readings
    }
    
    void loop () {
            
      static unsigned long lastAttempt = 0;
      
      if (WiFi.status() != WL_CONNECTED && millis() - lastAttempt > 10000) {
        lastAttempt = millis();
        
        logMsg = "WiFi disconnected. Reconnecting...";
        Serial.println(logMsg);
        
        connectToWiFi();
        mqttLog(logMsg);
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