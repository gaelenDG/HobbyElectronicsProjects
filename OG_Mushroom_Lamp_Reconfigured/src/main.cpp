#include <Arduino.h>
#include <esp_sleep.h>

#define RED_PIN 1
#define BLUE_PIN 2
#define Amanita_PIN 6
#define GREEN_PIN 7
#define Flickering_PIN 8        // The funny flickering light
#define PHOTO_PIN 0

const int darknessThreshold = 3000;    // Threshold for darkness detection
const int minBrightness = 25;         // Minimum brightness level
const int maxBrightness = 255;        // Maximum brightness level
const int constantBrightness = 255;   // Fixed brightness for LED on pin 7
const unsigned long darkDebounceTime = 2000;  // Time to confirm darkness (in ms)
unsigned long darkDetectedTime = 0;           // Time when darkness was first detected
const int numFadingLEDs = 4;
bool isDark = false;

struct LEDControl {
  int pin;
  int brightness;
  int fadeAmount;
  unsigned long lastUpdate;
  int fadeInterval;
  int LEDmaxBrightness;
};

LEDControl leds[] = {
  {RED_PIN, minBrightness, 1, 0, 0, 255},
  {BLUE_PIN, minBrightness, 1, 0, 0, 150},
  {Amanita_PIN, minBrightness, 1, 0, 0, 255},
  {GREEN_PIN, minBrightness, 1, 0, 0, 255}
};


// Function prototypes
void updateLED(int index);
void resetLEDs();
void enterLowPowerMode();


void setup() {
  Serial.begin(115200);

  for (int i = 0; i < numFadingLEDs; i++) {
    pinMode(leds[i].pin, OUTPUT);
    leds[i].lastUpdate = millis();
    leds[i].fadeInterval = random(2000, 10000);
  }

  pinMode(Flickering_PIN, OUTPUT);
  analogWrite(Flickering_PIN, constantBrightness);
}


void loop() {
  int lightLevel = analogRead(PHOTO_PIN);
  Serial.println(lightLevel);

  // Check if it’s below the threshold and set the timing buffer
  if (lightLevel < darknessThreshold) {
    if (!isDark && darkDetectedTime == 0) {
      darkDetectedTime = millis();  // Start timing if darkness is detected
    } else if (!isDark && (millis() - darkDetectedTime > darkDebounceTime)) {
      // If it’s been dark for the debounce time, turn on the LEDs
      resetLEDs();
      analogWrite(Flickering_PIN, constantBrightness); // Start the flickering light
      isDark = true;
      darkDetectedTime = 0;
    }
  } else {
    darkDetectedTime = 0;  // Reset timing if light is above threshold
    if (isDark) {
      // Turn off LEDs if transitioning from dark to light
      for (int i = 0; i < numFadingLEDs; i++) {
        analogWrite(leds[i].pin, 0);
      }
      analogWrite(Flickering_PIN, 0); // Shut off the flickering light
      isDark = false;

      // Enter low-power mode until the light level changes
      enterLowPowerMode();
    }
  }

  // Update each fading LED independently if in darkness
  if (isDark) {
    for (int i = 0; i < numFadingLEDs; i++) {
      updateLED(i);
    }
  }

  delay(20); // A pause to give the update function time for perceived changes
}


// float elapsedTime = (currentMillis - startTime[i]) % cycleDuration[i]; // Loop within cycle
//             float phase = (elapsedTime / (float)cycleDuration[i]) * 2 * PI;  // Convert time to radians
//             brightness[i] = (sin(phase - PI) + 1.0) / 2.0; // Reverse sine wave phase (min to max)

//             int scaledBrightness = (brightness[i] * 255); // Scale from 25 to 255


void updateLED(int index) {
  unsigned long currentMillis = millis();

  float elapsedTime = (currentMillis - leds[index].lastUpdate);
  float phase = (elapsedTime / (float)leds[index].fadeInterval) * 2 * PI; // calculate phase in radians

  // Sine wave oscillates between -1 and 1, so scale to [0, 1]
  float sineValue = (sin(phase - PI/2) + 1.0) / 2.0;

  // Scale to desired brightness range
  int scaledBrightness = minBrightness + (int)(sineValue * (leds[index].LEDmaxBrightness - minBrightness));

  leds[index].brightness = scaledBrightness;

  // If one cycle is complete, randomize fadeInterval and reset lastUpdate
  if (elapsedTime > leds[index].fadeInterval) {
    leds[index].fadeInterval = random(2000, 10000);
    leds[index].lastUpdate = currentMillis;
  }

  // leds[index].lastUpdate = currentMillis;
  analogWrite(leds[index].pin, leds[index].brightness);
}


// void updateLED(int index) {
//   unsigned long currentMillis = millis();

//   if (currentMillis - leds[index].lastUpdate >= leds[index].fadeInterval / (maxBrightness - minBrightness)) {
//     leds[index].lastUpdate = currentMillis;
//     leds[index].brightness += leds[index].fadeAmount;

//     if (leds[index].brightness <= minBrightness || leds[index].brightness >= maxBrightness) {
//       leds[index].fadeAmount = -leds[index].fadeAmount;
//       leds[index].fadeInterval = random(2000, 10000);
//     }

//     analogWrite(leds[index].pin, leds[index].brightness);
//   }
// }

void resetLEDs() {
  for (int i = 0; i < numFadingLEDs; i++) {
    leds[i].brightness = minBrightness;
    leds[i].fadeAmount = 1;
    leds[i].lastUpdate = millis();
    leds[i].fadeInterval = random(2000, 10000);
    analogWrite(leds[i].pin, leds[i].brightness);
  }

  analogWrite(Flickering_PIN, constantBrightness);
}

void enterLowPowerMode() {
  Serial.println("Entering low-power mode...");

  // Set wakeup condition based on light level on PHOTO_PIN
  esp_sleep_enable_timer_wakeup(5000); // Wake up every 500 ms to check the light level
  esp_light_sleep_start();               // Enter light sleep
}