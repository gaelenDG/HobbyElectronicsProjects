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
      case 1:  initPattern1(); break; // no real pattern, just all lights on
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
  lightStrand(0);
}

// A basic light, all equally bright nice yellow light
void initPattern1() {
  // Apply global brightness limit (0–100 from MQTT)
  int scaledBrightness = 255 * maxBrightness / 100;
  
  // Scale RGB
  int red   = (255 * scaledBrightness) / 255;
  int green = (150 * scaledBrightness) / 255;
  int blue  = (50 * scaledBrightness) / 255;
  int white = (255 * scaledBrightness) / 255;
  
  int strand = (255 * scaledBrightness) / 255;
  
  // Apply scaled light to all pixels in the chain
  for (int i = 0; i < NUM_PIXELS; i++) {
    lightPixel(i, red, green, blue, white);
  }
  
  lightStrand(strand);
}

// ======== Pattern 2 ========
// All pixels shift smoothly from red to green to blue to red

void updatePattern2() {
  Serial.println("Beginning Pattern2!");
  
  // Starting point for pattern
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
    
    // Apply global brightness limit (0–100 from MQTT)
    int scaledBrightness = 255 * maxBrightness / 100;
    
    // Scale RGB
    int red_out   = (red * scaledBrightness) / 255;
    int green_out = (green * scaledBrightness) / 255;
    int blue_out  = (blue * scaledBrightness) / 255;
    
    for (int i = 0; i < 8; i++) {
      lightPixel(i, red_out, green_out, blue_out, white);
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
  
  // Apply global brightness limit (0–100 from MQTT)
  int scaledBrightness = 255 * maxBrightness / 100;
  
  // Scale RGB
  int green = (255 * scaledBrightness) / 255;
  
  for (int i = 0; i < NUM_PIXELS; i++) {
    lightPixel(i, 0, green, 0, 0);
  }
  
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
// void initPattern4() {
//   Serial.println("Starting Breathing Pattern");

//   for (int i = 0; i < NUM_PIXELS; i++) {
//     startTime_arr[i] = millis();
//     cycleDuration_arr[i] = random(5000, 20000);
//     waitingForMin_arr[i] = true;

//     pickRandomColor(colors_arr[i][0], colors_arr[i][1], colors_arr[i][2]);
//     pickRandomColor(newColors_arr[i][0], newColors_arr[i][1], newColors_arr[i][2]);
//     phase_arr[i] = 0;
//   }

//   // Initialize strand breathing 
//   startTime_strand = millis();
//   cycleDuration_strand = random(5000, 20000);
//   phase_strand = 0;
// }

void initPattern4() {
  Serial.println("Starting Breathing Pattern");
  
  for (int i = 0; i < NUM_PIXELS; i++) {
    startTime_arr[i] = millis();
    cycleDuration_arr[i] = random(5000, 20000);
    
    pickRandomColor(
      colors_arr[i][0], colors_arr[i][1], colors_arr[i][2]
    );
    
    pickRandomColor(
      newColors_arr[i][0], newColors_arr[i][1], newColors_arr[i][2]
    );
  }
  
  startTime_strand = millis();
  cycleDuration_strand = random(5000, 20000);
}

// void updatePattern4() {
//   static unsigned long previousMillis = 0;
//   const unsigned long interval = 10;

//   unsigned long currentMillis = millis();
//   if (currentMillis - previousMillis < interval) return;

//   previousMillis = currentMillis;

//   for (int i = 0; i < NUM_PIXELS; i++) {
//     float prevBrightness = brightness_arr[i];

//     // Smooth continuous sine wave (no modulo)
//     float phase = ((currentMillis - startTime_arr[i]) / (float)cycleDuration_arr[i]) * 2 * PI;
//     brightness_arr[i] = (sin(phase - PI) + 1.0) / 2.0;

//     float phaseNorm = (currentMillis - startTime_arr[i]) / (float)cycleDuration_arr[i];

//     float prevPhase = phase_arr[i]; // store this per pixel (you need this array)

//     phase_arr[i] = fmod((currentMillis - startTime_arr[i]) / (float)cycleDuration_arr[i], 1.0);

//     bool wrapped = (phase_arr[i] < prevPhase);

//     // Detect minimum crossing (rising from near-zero)
//     // if (prevBrightness < 0.02 && brightness_arr[i] > prevBrightness) {
//     // if (wasDecreasing_arr[i] && !decreasing) {
//     if (wrapped) {
//       colors_arr[i][0] = newColors_arr[i][0];
//       colors_arr[i][1] = newColors_arr[i][1];
//       colors_arr[i][2] = newColors_arr[i][2];

//       pickRandomColor(
//         newColors_arr[i][0],
//         newColors_arr[i][1],
//         newColors_arr[i][2]
//       );
//       // Set new breathing interval
//       cycleDuration_arr[i] = random(10000, 20000);

//       // startTime_arr[i] = currentMillis - cycleDuration_arr[i] / 4;
//     }

//     // ===== Gamma correction (perceptual brightness) =====
//     // Gamma ~2.2 is typical for LEDs
//     float gamma = 2.2;
//     float corrected = 0.03 + 0.97 * pow(brightness_arr[i], gamma);

//     // Apply global brightness limit (0–100 from MQTT)
//     int scaledBrightness = corrected * 255 * maxBrightness / 100;

//     // Scale RGB
//     int red   = (colors_arr[i][0] * scaledBrightness) / 255;
//     int green = (colors_arr[i][1] * scaledBrightness) / 255;
//     int blue  = (colors_arr[i][2] * scaledBrightness) / 255;

//     lightPixel(i, red, green, blue, 0);
//   }

//   // strand breathing
//   float prevBrightness_strand = brightness_strand;

//   // Smooth continuous sine wave (no modulo)
//   float phase = ((currentMillis - startTime_strand) / (float)cycleDuration_strand) * 2 * PI;
//   brightness_strand = (sin(phase - PI) + 1.0) / 2.0;

//   // bool decreasing = brightness_strand < prevBrightness_strand;
//   float phaseNorm = (currentMillis - startTime_strand) / (float)cycleDuration_strand;

//   float prevPhaseStrand = phase_strand;

//   phase_strand = fmod((currentMillis - startTime_strand) / (float)cycleDuration_strand, 1.0);

//   bool wrapped = (phase_strand < prevPhaseStrand);

//   // Detect minimum crossing (rising from near-zero)
//   // if (prevBrightness_strand < 0.02 && brightness_strand > prevBrightness_strand) {
//   // if (wasDecreasing_strand && !decreasing) {
//   if (phase_strand < prevPhaseStrand) {
//     cycleDuration_strand = random(10000, 20000);
//     // startTime_strand = currentMillis - cycleDuration_strand / 4;
//   }

//   // ===== Gamma correction (perceptual brightness) =====
//   // Gamma ~2.2 is typical for LEDs
//   float gamma = 2.2;
//   float corrected = 0.03 + 0.97 * pow(brightness_strand, gamma);

//   // Apply global brightness limit (0–100 from MQTT)
//   int scaledBrightness = corrected * 255 * maxBrightness / 100;

//   // Scale PWM
//   int pwm = corrected * 255 * maxBrightness / 100;

//   lightStrand(pwm);

//   NeoPixelChain.show();
// }
void updatePattern4() {
  static unsigned long lastUpdate = 0;
  const unsigned long interval = 10;

  unsigned long now = millis();
  if (now - lastUpdate < interval) return;
  float dt = (now - lastUpdate) / 1000.0f;
  lastUpdate = now;

  float gamma = 2.2;

  // =========================
  // NEOPIXELS
  // =========================
  for (int i = 0; i < NUM_PIXELS; i++) {

    // advance phase manually (THIS is the key fix)
    phase_arr[i] += dt / (cycleDuration_arr[i] / 1000.0f);

    // detect clean wrap
    if (phase_arr[i] >= 1.0f) {
      phase_arr[i] -= 1.0f;

      // swap colors EXACTLY once per cycle
      colors_arr[i][0] = newColors_arr[i][0];
      colors_arr[i][1] = newColors_arr[i][1];
      colors_arr[i][2] = newColors_arr[i][2];

      pickRandomColor(
        newColors_arr[i][0],
        newColors_arr[i][1],
        newColors_arr[i][2]
      );

      cycleDuration_arr[i] = random(10000, 20000);
    }

    // sine wave from stable phase
    float brightness = 0.5f * (1.0f + sin(phase_arr[i] * TWO_PI - HALF_PI));

    float corrected = pow(brightness, gamma);
    int scaled = corrected * 255 * maxBrightness / 100;

    lightPixel(
      i,
      (colors_arr[i][0] * scaled) / 255,
      (colors_arr[i][1] * scaled) / 255,
      (colors_arr[i][2] * scaled) / 255,
      0
    );
  }

  // =========================
  // STRAND
  // =========================

  phase_strand += dt / (cycleDuration_strand / 1000.0f);

  if (phase_strand >= 1.0f) {
    phase_strand -= 1.0f;
    cycleDuration_strand = random(10000, 20000);
  }

  float brightness = 0.5f * (1.0f + sin(phase_strand * TWO_PI - HALF_PI));

  float corrected = pow(brightness, gamma);
  int pwm = corrected * 255 * maxBrightness / 100;

  lightStrand(pwm);

  NeoPixelChain.show();
}