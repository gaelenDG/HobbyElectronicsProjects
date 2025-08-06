#include <Adafruit_NeoPixel.h>
#include <vector>
#include <iostream>
using namespace std;

// NeoPixel setup
#define NEOPIXEL_PIN1 6       // GPIO pin for first NeoPixel chain
#define NEOPIXEL_PIN2 7       // GPIO pin for second NeoPixel chain
#define NUM_PIXELS 12         // Number of NeoPixels per chain
#define LED_BUILTIN 10

Adafruit_NeoPixel pixels1(NUM_PIXELS, NEOPIXEL_PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2(NUM_PIXELS, NEOPIXEL_PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel BUILTIN(1, LED_BUILTIN, NEO_GRB + NEO_KHZ800);

// Photoresistor setup
#define PHOTORESISTOR_COUNT 5
const int photoResistorPins[PHOTORESISTOR_COUNT] = {0, 1, 2, 3, 4};

// Thresholds for detecting significant light changes
int lightThresholds[PHOTORESISTOR_COUNT];
const int buffer = 50; // Sensitivity buffer for activation

// Declare the graph globally so it is accessible everywhere
vector<int> graph[24] = {
  {1, 5, 6, 7},
  {0, 2, 3, 4, 5},
  {1, 3, 14},
  {1, 2, 4, 11},
  {1, 3, 5, 9, 10, 11},
  {0, 1, 4, 6, 9},
  {0, 5, 7, 8, 9},
  {0, 6, 8},
  {6, 7, 9},
  {4, 5, 6, 8},
  {4, 9, 11},
  {3, 4, 10},
  {13, 17, 18, 19},
  {12, 14, 15, 16, 17},
  {2, 13, 15, 16},
  {13, 14, 16, 23},
  {13, 15, 17, 21, 22, 23},
  {12, 13, 16, 18, 21},
  {12, 17, 19, 20, 21},
  {12, 18, 20},
  {18, 19, 21},
  {16, 17, 18, 20},
  {16, 21, 23},
  {15, 16, 22}
};

// Function prototypes
void setLEDs(Adafruit_NeoPixel &pixels, const uint32_t colors[]);
void calibrateThresholds();
int determinePattern();
void printPhotoResistorValues(int activeCount);

// Pattern functions
void patternSequential(Adafruit_NeoPixel &pixels, int currentPattern);
void patternStaticColors(Adafruit_NeoPixel &pixels, const uint32_t colors[]);
void patternRandomWalk(Adafruit_NeoPixel &pixels, int currentPattern);

void setup() {
  // Initialize NeoPixels
  pixels1.begin();
  pixels2.begin();
  BUILTIN.begin();

  pixels1.setBrightness(25);
  pixels2.setBrightness(25);
  BUILTIN.setPixelColor(0, BUILTIN.Color(0, 255, 0));
  pixels1.show();
  pixels2.show();
  BUILTIN.show();

  // Initialize photoresistors
  for (int i = 0; i < PHOTORESISTOR_COUNT; i++) {
    pinMode(photoResistorPins[i], INPUT);
  }

  Serial.begin(115200);
  Serial.println("System initializing...");

  calibrateThresholds();
  Serial.println("Threshold calibration complete. System ready.");
}
void loop() {
  int activePattern = determinePattern();
  static int lastPattern = -1;

  // Only update if the pattern changes
  if (activePattern != lastPattern) {
    lastPattern = activePattern;
    Serial.print("Page open: ");
    Serial.println(activePattern);

    // Execute the corresponding pattern
    switch (activePattern) {
      case 0:
        // setting all neopixels to a light green color to start
        for (int i = 0; i < NUM_PIXELS; i++) {
          pixels1.setPixelColor(i, pixels1.Color(10, 20, 0));
          pixels2.setPixelColor(i, pixels2.Color(10, 20, 0));
        }
        pixels1.show();
        pixels2.show();
        break;

      case 1:
        pixels1.clear();
        pixels2.clear();
        pixels1.show();
        pixels2.show();
        patternSequential(pixels1, 1);
        patternSequential(pixels2, 1);
        break;

      case 2: {
        patternRandomWalk(pixels1, pixels2, 2);
      }
      case 3: {
        patternSpreadingWave(pixels1, pixels2, 3);
      }
      case 4: {}
      case 5: {}
    }
  }
// Continuously check for photoresistor changes
  printPhotoResistorValues(activePattern);
}


// Calibrate light thresholds for each photoresistor
void calibrateThresholds() {
  for (int i = 0; i < PHOTORESISTOR_COUNT; i++) {
    int rawValue = analogRead(photoResistorPins[i]);
    lightThresholds[i] = rawValue + buffer;
    Serial.print("Photoresistor ");
    Serial.print(i);
    Serial.print(" threshold: ");
    Serial.println(lightThresholds[i]);
  }
}

// Determine which pattern to display based on photoresistor activation
int determinePattern() {
    int activeCount = 0;
    for (int i = 0; i < PHOTORESISTOR_COUNT; i++) {
        int rawValue = analogRead(photoResistorPins[i]);
        if (rawValue > lightThresholds[i]) {
            activeCount++;
        }
    }
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


// Set NeoPixels to a specific color pattern
void patternStaticColors(Adafruit_NeoPixel &pixels, const uint32_t colors[]) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, colors[i]);
  }
  pixels.show();  // Update NeoPixel strip
}

// Pattern: Sequential lighting
void patternSequential(Adafruit_NeoPixel &pixels, int currentPattern) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (determinePattern() != currentPattern) {
            return; // Exit if pattern changes
        }
        pixels.clear();
        pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Green
        pixels.show();
        delay(500);
    }
}

// Pattern: Random walk
void patternRandomWalk(Adafruit_NeoPixel &pixels1, Adafruit_NeoPixel &pixels2, int currentPattern) {

  // Starting node will always be position 0
  static int node = 0;

  // Monitor for whether the current page is correct
  int activePattern = determinePattern();

  // Start page behavior with all pixels off
  pixels1.clear();
  pixels2.clear();

  // Define the color of the NeoPixel to travel across the page
  uint32_t color = pixels1.Color(0, 10, 0);

  // While loop determines if the correct page is still open, keeps the pattern going if true
  while (activePattern == currentPattern) {

    // Determine which light on which chain is the proper one to illuminate for the current node
    if (node >= 0 && node < NUM_PIXELS) {
      // Chain1: Nodes 0-(NUM_PIXELS-1)
      pixels1.setPixelColor(node, color);
      pixels1.show();
    } else if (node >= NUM_PIXELS && node < 2 * NUM_PIXELS) {
      // Chain2: Nodes NUM_PIXELS-(2*NUM_PIXELS-1)
      pixels2.setPixelColor(node - NUM_PIXELS, color);
      pixels2.show();
    }

    // Keeps the light on for 1 sec before moving on
    delay(500);

    // Find a new node neighbor to go to
    int nextNode = graph[node][random(0, graph[node].size())];

    // Update the serial monitor with the move to be made, to help determine if things are working properly
    Serial.print("Moving from ");
    Serial.print(node);
    Serial.print(" to ");
    Serial.println(nextNode);
    node = nextNode;

    // Turn all the pixels off to prepare for the next iteration
    pixels1.clear();
    pixels2.clear();

    // Determine which page is open, update the while loop
    activePattern = determinePattern();
  }
}

// Pattern: Spreading wave
void patternSpreadingWave(Adafruit_NeoPixel &pixels1, Adafruit_NeoPixel &pixels2, int currentPattern) {

  // Monitor for whether the correct page is open
  int activePattern = determinePattern();


  // Clear all pixels at the start of a new wave
  pixels1.clear();
  pixels2.clear();
  pixels1.show();
  pixels2.show();

  // While loop keeps the pattern going as long as the correct page is open
  while (activePattern == currentPattern) {


    // Choose a random starting node
    int startNode = random(0, 2 * NUM_PIXELS);

    // Generate a random color for the wave
    uint32_t color = pixels1.Color(random(0, 256), random(0, 256), random(0, 256));

    // Array to track visited nodes
    bool visited[2 * NUM_PIXELS] = {false};

    // Simple queue implementation using an array
    int queue[2 * NUM_PIXELS]; // Maximum possible size is the total number of nodes
    int front = 0, back = 0;

    // Enqueue the starting node
    queue[back++] = startNode;
    visited[startNode] = true;

    // Begin the wave propagation
    while (front < back) {
      int currentNode = queue[front++]; // Dequeue the current node

      // Determine which chain and pixel index to light up
      if (currentNode >= 0 && currentNode < NUM_PIXELS) {
        pixels1.setPixelColor(currentNode, color);
        pixels1.show();
      } else if (currentNode >= NUM_PIXELS && currentNode < 2 * NUM_PIXELS) {
        pixels2.setPixelColor(currentNode - NUM_PIXELS, color);
        pixels2.show();
      }

      // Add a small delay for the wave effect
      delay(200);

      // Enqueue all unvisited neighbors
      for (int neighbor : graph[currentNode]) {
        if (!visited[neighbor]) {
          queue[back++] = neighbor;
          visited[neighbor] = true;
        }
      }
    }

    // Delay for a moment before restarting the wave with a new random color
    delay(1000);

    // Determine which page is open, update the while loop
    activePattern = determinePattern();
  }
}




