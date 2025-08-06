// ======== Library Initialization ========

#include <Adafruit_NeoPixel.h>
#include <vector>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <esp_task_wdt.h>
#include <helpers.h>
#include <config.h>
using namespace std;

// ======== Setup ========

void setup() {

  // Serial Monitor
  Serial.begin(9600);
  while(!Serial); // Wait for Serial Monitor to open
  Serial.println("System initializing...");

  // Initialize NeoPixels
  Chain1.begin();
  Chain2.begin();
  StatusLight.begin();

  StatusLight.setPixelColor(0, StatusLight.Color(10, 10, 0)); // Red component (GRB format)
  StatusLight.show();

  // Initialize photoresistors
  for (int i = 0; i < PHOTORESISTOR_COUNT; i++) {
    pinMode(photoResistorPins[i], INPUT);
  }

  Serial.println("System ready.");

}

// ======== Main Loop ========

void loop() {
  int activePattern = determinePage();
  static int lastPattern = -1;

  esp_task_wdt_reset(); // Reset watchdog timer in each loop iteration

  // Only update if the pattern changes
  if (activePattern != lastPattern) {
    lastPattern = activePattern;
    Serial.print("Page open: ");
    Serial.println(activePattern);

    // Execute the corresponding pattern
    switch (activePattern) {
      case 0:
        multipleRandomWalk(0);

      case 1:
        patternSpreadingWave(1);

      case 2:
      multipleRandomWalk(2);

      case 3:
        patternSpreadingWave(3);

      case 4:
      multipleRandomWalk(4);
    }
  }
}