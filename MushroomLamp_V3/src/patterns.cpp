// ======== Includes =======
#include "helpers.h"
#include "config.h"
#include <vector>
#include <queue>
#include <math.h>  // required for sin() function
#include <unordered_map>
using namespace std;


// The overarching set pattern function
void updateCurrentPattern() {
  if (previousPattern == patternIndex) {
    
    switch (patternIndex) {
      case 0:  break; // no pattern, just all lights off - should be handled fine by init
      case 1:  break; // no real pattern, just all lights on
      case 2: updatePattern2(); break;
      case 3: updatePattern3(); break;
      case 4: updatePattern4(); break;
    }
  } else { // if previous does not match current, trigger appropriate init
    switch (patternIndex) {
      case 0: DefaultPattern(); break;  
      case 1: initPattern1(); break;
      case 2: updatePattern2(); break; // Nothing to init, just trigger start of pattern
      case 3: updatePattern3(); break;// Nothing to init, just trigger start of pattern
      case 4: initPattern4(); break;
    }
    
    previousPattern = patternIndex;
  }
  
}

// All lights off
void DefaultPattern(){
  NeoPixelChain.clear();
  NeoPixelChain.show();
}

// A basic light, all equally bright nice yellow light
void initPattern1() {
  for (int i = 0; i < NUM_PIXELS; i++) {
    lightPixel(i, 255, 150, 50, 255);
  }
}

// ======== Pattern 2 ========
// All pixels shift smoothly from red to green to blue to red

void updatePattern2() {
  Serial.println("Beginning Pattern2!");
  
  static int red = 255, green = 0, blue = 0, white = 0;
  static unsigned long previousMillis = 0;
  const unsigned long interval = 50;
  int fadeSpeed = 1;
  
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Same color transition logic
    if (red == 255 && green < 255 && blue == 0) {
      green += fadeSpeed;
    } else if (green == 255 && red > 0 && blue == 0) {
      red -= fadeSpeed;
    } else if (green == 255 && red == 0 && blue < 255) {
      blue += fadeSpeed;
    } else if (blue == 255 && green > 0 && red == 0) {
      green -= fadeSpeed;
    } else if (blue == 255 && green == 0 && red < 255) {
      red += fadeSpeed;
    } else if (red == 255 && blue > 0 && green == 0) {
      blue -= fadeSpeed;
    }
    
    for (int i = 0; i < 8; i++) {
      lightPixel(i, red, green, blue, white);
    }
  }
}

// Pattern 3: turning on a rainbow
void updatePattern3() {
  const unsigned long interval = 500;  // Interval between pixel updates
  unsigned long previousMillis = 0;   // Tracks last update time
  unsigned long offMillis = 0;        // Tracks when to turn off pixels
  
  unsigned long currentMillis = millis();  // starts tracking time
  
  NeoPixelChain.clear();
  NeoPixelChain.show();
  
  int currentPixel = 0;               // Track which pixel is being lit
  bool turningOff = false;            // Track if we are in the turn-off phase
  
  for (int i = 0; i < NUM_PIXELS; i++) {
    lightPixel(i, 0, 150, 0, 0);
  }
  
  // while (!buttonInterrupt) {
  //   // int buttonPresses = getButtonPresses();
  
  //   unsigned long currentMillis = millis(); // Continuously update millis()
  
  //   // Light up pixels one by one
  //   if (!turningOff && (currentMillis - previousMillis >= interval)) {
  //     previousMillis = currentMillis; // Reset timer
  
  //     int hue = (currentPixel * 256 / NUM_PIXELS);  // Spread hues across pixels
  //     uint32_t color = StatusLight.ColorHSV(hue * 256, 255, 150); // Convert to NeoPixel color
  //     lightPixel(currentPixel, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, 0);
  
  //     currentPixel++;  // Move to the next pixel
  
  //     // Once all pixels are lit, start turn-off sequence
  //     if (currentPixel >= NUM_PIXELS) {
  //       turningOff = true;
  //       currentPixel = 0;
  //       offMillis = millis(); // Mark start of turn-off sequence
  //     }
  //   }
  
  //   // Turn pixels off in sequence
  //   if (turningOff && (currentMillis - offMillis >= interval)) {
  //     offMillis = currentMillis; // Reset timer
  
  //     lightPixel(currentPixel, 0, 0, 0, 0); // Turn off current pixel
  //     currentPixel++;
  
  //     // Once all pixels are turned off, restart the sequence
  //     if (currentPixel >= NUM_PIXELS) {
  //       turningOff = false;
  //       currentPixel = 0;
  //     }
  //   }
  
  //   // Exit the pattern if button is pressed again
  //   if (buttonInterrupt) {
  //     Serial.println("Button pressed! Exiting Pattern3...");
  //     // buttonInterrupt = false;  // Reset flag
  //     return;
  //   }
  // }
}

// Pattern 4: Independent breathing for Each Pixel

// Choose a random color from the color options
void pickRandomColor(int &r, int &g, int &b) {
  int index = random(numColors);  // Pick random index from available colors
  r = colorOptions[index][0];
  g = colorOptions[index][1];
  b = colorOptions[index][2];
}

// On starting pattern 4, choose initial vals
void initPattern4() {
  Serial.println("Starting Breathing Pattern");
  
  for (int i = 0; i < NUM_PIXELS; i++) {
    startTime_arr[i] = millis();
    cycleDuration_arr[i] = random(5000, 20000);
    waitingForMin_arr[i] = true;
    
    pickRandomColor(colors_arr[i][0], colors_arr[i][1], colors_arr[i][2]);
    pickRandomColor(newColors_arr[i][0], newColors_arr[i][1], newColors_arr[i][2]);
    
    wasDecreasing_arr[i] = true; 
  }
}

void updatePattern4() {
  static unsigned long previousMillis = 0;
  const unsigned long interval = 20;

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis < interval) return;
  previousMillis = currentMillis;

  for (int i = 0; i < NUM_PIXELS; i++) {
    float prevBrightness = brightness_arr[i];

    // Smooth continuous sine wave (no modulo)
    float phase = ((currentMillis - startTime_arr[i]) / (float)cycleDuration_arr[i]) * 2 * PI;
    brightness_arr[i] = (sin(phase - PI) + 1.0) / 2.0;

    // Detect minimum crossing (rising from near-zero)
    if (prevBrightness < 0.02 && brightness_arr[i] > prevBrightness) {
      colors_arr[i][0] = newColors_arr[i][0];
      colors_arr[i][1] = newColors_arr[i][1];
      colors_arr[i][2] = newColors_arr[i][2];

      pickRandomColor(newColors_arr[i][0], newColors_arr[i][1], newColors_arr[i][2]);
    }

    // ===== Gamma correction (perceptual brightness) =====
    // Gamma ~2.2 is typical for LEDs
    float gamma = 2.2;
    float corrected = pow(brightness_arr[i], gamma);

    // Apply global brightness limit (0–100 from MQTT)
    int scaledBrightness = corrected * 255 * maxBrightness / 100;

    // Scale RGB
    int red   = (colors_arr[i][0] * scaledBrightness) / 255;
    int green = (colors_arr[i][1] * scaledBrightness) / 255;
    int blue  = (colors_arr[i][2] * scaledBrightness) / 255;

    lightPixel(i, red, green, blue, 0);
  }

  NeoPixelChain.show();
}
