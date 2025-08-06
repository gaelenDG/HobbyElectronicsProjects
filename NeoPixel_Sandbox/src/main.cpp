#include Adafruit_NeoPixel.h

#define NEOPIXEL_PIN 5     // GPIO pin where NeoPixel data line is connected
#define NUMPIXELS 3        // Number of NeoPixels (changed to 2)
#define LED_BUILTIN 2      // Built-in LED pin on ESP32-C3

Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

int red = 25;        // Start with full red
int green = 0;        // No green at the start
int blue = 0;         // No blue at the start

int fadeSpeed = 1;    // Adjust this for faster/slower color transitions

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);      // Set built-in LED as output for confirmation
  digitalWrite(LED_BUILTIN, HIGH);   // Turn on built-in LED

  pixels.begin();                    // Initialize NeoPixel
}

void loop() {
  // Transition from red to green
  while (red > 0 && green < 25) {
    red -= fadeSpeed;
    green += fadeSpeed;

    // Set both pixels to the same color
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(red, green, blue));
    }
    pixels.show();
    delay(40);  // Delay to control transition speed
  }

  // Transition from green to blue
  while (green > 0 && blue < 25) {
    green -= fadeSpeed;
    blue += fadeSpeed;

    // Set both pixels to the same color
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(red, green, blue));
    }
    pixels.show();
    delay(40);
  }

  // Transition from blue to red
  while (blue > 0 && red < 25) {
    blue -= fadeSpeed;
    red += fadeSpeed;

    // Set both pixels to the same color
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(red, green, blue));
    }
    pixels.show();
    delay(40);
  }
}
