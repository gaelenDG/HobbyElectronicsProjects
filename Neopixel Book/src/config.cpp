// ======== Includes ========
#include <Adafruit_NeoPixel.h>
#include <vector>
#include "config.h"
using namespace std;

//  ======== GPIO Pin Definitions ========
const int PHOTORESISTOR_COUNT = 5;
const int photoResistorPins[] = {0, 1, 2, 3, 4};

//  ======== NeoPixel Declarations ========
Adafruit_NeoPixel StatusLight(1, 10, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Chain1(12, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel Chain2(12, 7, NEO_GRB + NEO_KHZ800);
int NUM_PIXELS = 24;

//  ======== Constants ========

//  A simple adjacency map representing connections between NeoPixels.
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
  {4, 5, 6, 8, 21},
  {4, 9, 11, 22},
  {3, 4, 10, 23},
  {13, 17, 18, 19},
  {12, 14, 15, 16, 17},
  {2, 13, 15, 16},
  {13, 14, 16, 23},
  {13, 15, 17, 21, 22, 23},
  {12, 13, 16, 18, 21},
  {12, 17, 19, 20, 21},
  {12, 18, 20},
  {9, 18, 19, 21},
  {10, 16, 17, 18, 20},
  {11, 16, 21, 23},
  {15, 16, 22}
};

const int lightThreshold = 100; // Thresholds for detecting significant light changes
const int buffer = 50; // Sensitivity buffer for activation


//  Color Options
/*
 * Predefined color options for the NeoPixels, represented as RGB values.
 * The array stores different colors that can be randomly selected for lighting patterns.
 */
 const int colorOptions[][3] = {
  {50, 0, 0},    // Red
  {0, 50, 0},    // Green
  {0, 0, 50},    // Blue
  {50, 50, 0},   // Yellow
  {0, 50, 50},   // Cyan
  {50, 0, 50},   // Magenta
  {50, 32, 0},   // Orange
  {14, 0, 25}    // Indigo
};

/*
* Computes the number of available color options dynamically.
* This ensures the size of colorOptions is correctly calculated for use in random selection.
*/
const int numColors = sizeof(colorOptions) / sizeof(colorOptions[0]);
