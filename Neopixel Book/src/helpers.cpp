// ======== Includes ========
# include "helpers.h"
# include "config.h"
# include <Arduino.h>
# include <Adafruit_NeoPixel.h>

// ======== Helper Functions ========

// Determine which pattern to display based on photoresistor activation
int determinePage() {
  int activeCount = 0;
  for (int i = 0; i < PHOTORESISTOR_COUNT; i++) {
      int rawValue = analogRead(photoResistorPins[i]);
      if (rawValue > lightThreshold) {
          activeCount++;
      }
  }
  // esp_task_wdt_reset();
  return activeCount; // Assuming activeCount maps to a pattern
}

// Print photoresistor values in a single line if exceeding thresholds
void printPhotoResistorValues(int activeCount) {
  static int lastActiveCount = -1;

  // Only print when activeCount changes
  if (activeCount != lastActiveCount) {
    lastActiveCount = activeCount;
    Serial.print("Photoresistor values: ");
    for (int i = 0; i < PHOTORESISTOR_COUNT; i++) {
      int rawValue = analogRead(photoResistorPins[i]);
      Serial.print(rawValue);
      if (i < PHOTORESISTOR_COUNT - 1) Serial.print(", ");
    }
    Serial.println();
  }
}

/* Light up a specific NeoPixel at a given position with a specified color
  - Takes a position and a color array as arguments
  - The position is used to determine which chain and pixel to light up
  - The color array is used to set the RGB values for the pixel
*/
void lightPixel(int position, int Red, int Green, int Blue) {
  Adafruit_NeoPixel* chain = nullptr;  // Pointer to correct LED chain
  int localPosition = 0;  // Applies a pixel index that exists within the chains to help avoid errors.
  uint32_t color = 0;  // Store calculated color

  if (position >= 0 && position < 12) {
    chain = &Chain1;
    localPosition = position;

    color = chain->Color(Red, Green, Blue); // initializes a color to apply the RGB values to the pixel
  } else if (position >= 12 && position < 24) {
    chain = &Chain2;
    localPosition = position - 12;

    color = chain->Color(Red, Green, Blue); // initializes a color to apply the RGB values to the pixel
  }

  if (!chain) {
    Serial.print("Invalid position: ");
    Serial.println(position);
    return;
  }

  chain->setPixelColor(localPosition, color);
  chain->show();
}

/*
 * Selects a random color from a predefined list.
 * @param r Reference to red value (0-255)
 * @param g Reference to green value (0-255)
 * @param b Reference to blue value (0-255)
 */
 void pickRandomColor(int &r, int &g, int &b) {
  int index = random(numColors);  // Pick random index from available colors
  r = colorOptions[index][0];
  g = colorOptions[index][1];
  b = colorOptions[index][2];
}