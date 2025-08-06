// ======== Library initialization ========
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// ======== Function prototypes ========

// Defined in helpers.cpp
int determinePage();
void printPhotoResistorValues(int activeCount);
void lightPixel(int position, int Red, int Green, int Blue);
void pickRandomColor(int &r, int &g, int &b);

// Defined in patterns.cpp
void multipleRandomWalk(int currentPattern);
void multipleRandomWalkWithRainbow(int currentPattern);
void patternRainbowRandomWalk(int currentPattern); 
void patternSpreadingWave(int currentPattern);
void patternRandomWalk(int currentPattern);
void predatorPreyPattern(int currentPattern);

