#ifndef CONFIG_H
#define CONFIG_H

#include <Adafruit_NeoPixel.h>
#include <vector>

// ======== NeoPixel Declarations ========
extern Adafruit_NeoPixel StatusLight;  // Status light on the ESP32
extern Adafruit_NeoPixel Chain1;       // GPIO of front cover
extern Adafruit_NeoPixel Chain2;       // GPIO of back cover
extern int NUM_PIXELS;                  // Number of NeoPixels

// ======== Photoresistor Declarations ========
extern const int PHOTORESISTOR_COUNT; 
extern const int photoResistorPins[]; 

extern const int lightThreshold; // Thresholds for detecting significant light changes
extern const int buffer; // Sensitivity buffer for activation


// ======== Graph and color Declarations ========

// Declare the adjacency list for 24 nodes
extern std::vector<int> graph[24];

// Color options
extern const int colorOptions[][3];  // Explicitly declare as a 2D array
extern const int numColors;          // Declare numColors for access in other files

#endif // CONFIG_H
