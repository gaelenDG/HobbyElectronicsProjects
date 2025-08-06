#include "helpers.h"
#include "config.h"
#include <Arduino.h>
#include <queue>
#include <vector>
#include <unordered_map>
#include <esp_task_wdt.h>
using namespace std;

// ======== Patterns ========
/* This file contains the code for the various patterns that the Pixels can display.
 - Each pattern is a function that takes an integer argument, which is the current pattern number -
    - the patterns include a loop-breaking call to check the number of activated LDRs, and escapes the pattern if the number is different than the assigned pattern integer.
 - The patterns are called in the main loop based on the current page number.
*/

// Random walk pattern with variable number of lights
void multipleRandomWalk(int currentPattern) {

  // Firefly color
  int Red = 10;
  int Green = 10;
  int Blue = 0;

  // Number of lights in the pattern
  const int numLights = 6;

  // Total number of nodes
  const int numNodes = 24;

  // Current positions of the lights
  int positions[numLights];

  // Occupied state for each node
  bool occupied[numNodes] = {false};

  // Initialize the positions of the lights at random, ensuring no collisions
  for (int i = 0; i < numLights; i++) {
      int randomPosition;
      do {
          randomPosition = random(0, numNodes);
      } while (occupied[randomPosition]);
      positions[i] = randomPosition;
      occupied[randomPosition] = true;
  }
  

  // Initialize active page 
  int activePattern = determinePage();

  // Loop until the page changes
  while (determinePage() == activePattern) {

    int activePattern = determinePage();
    // Clear all pixels before updating positions
    for (int i = 0; i < numNodes; i++) {
        lightPixel(i, 0, 0, 0); // Turn off all pixels
    }

    // Move each light to a random neighbor
    for (int i = 0; i < numLights; i++) {
      int currentPosition = positions[i];
      int nextPosition;

      // Find an unoccupied neighbor
      bool moved = false;
      for (int attempts = 0; attempts < graph[currentPosition].size(); attempts++) {
        int neighborIndex = random(0, graph[currentPosition].size());
        nextPosition = graph[currentPosition][neighborIndex];
        if (!occupied[nextPosition]) {
          occupied[currentPosition] = false;
          positions[i] = nextPosition;
          occupied[nextPosition] = true;
          moved = true;
          break;
          }
      }

      // If no move was possible, stay in place
      if (!moved) {
          nextPosition = currentPosition;
      }

      // Light up the current position
      lightPixel(nextPosition, Red, Green, Blue);
    }

    // Small delay for smoother animation
    delay(500);
  }

  // Clear lights when exiting the pattern
  for (int i = 0; i < numNodes; i++) {
    lightPixel(i, 0, 0, 0); // Turn off all pixels
  }
}

// Pattern: Spreading wave of colors
void patternSpreadingWave(int currentPattern) {

  // Initialize the current page value
  int activePattern = determinePage();

  // Initialize the color for the wave
  int color[3]; // Current RGB color

  // Start with all pixels off
  for (int i = 0; i < NUM_PIXELS; i++) {
    lightPixel(i, 0, 0, 0);
  }

  // Loop until the page changes
  while (activePattern == currentPattern) {

    // Choose a random starting node
    int startNode = random(0, NUM_PIXELS);

    // Select a random color from the color options
    pickRandomColor(color[0], color[1], color[2]);

    // Array to track visited nodes
    bool visited[NUM_PIXELS] = {false};

    // Simple queue implementation using an array
    int queue[2 * NUM_PIXELS]; // Maximum possible size is the total number of nodes
    int front = 0, back = 0;

    // Enqueue the starting node
    queue[back++] = startNode;
    visited[startNode] = true;

    // Begin the wave propagation
    while (front < back && activePattern == currentPattern) {
      int currentNode = queue[front++]; // Dequeue the current node

      // Determine which chain and pixel index to light up
      lightPixel(currentNode, color[0], color[1], color[2]);

      // Add a small delay for the wave effect
      delay(200);

      // Enqueue all unvisited neighbors
      for (int neighbor : graph[currentNode]) {
        if (!visited[neighbor]) {
          queue[back++] = neighbor;
          visited[neighbor] = true;
        }
      }

      // Determine which page is open, update the while loop
      activePattern = determinePage();
    }

    // Delay for a moment before restarting the wave with a new random color
    delay(1000);

    // Determine which page is open, update the while loop
    activePattern = determinePage();
    if (activePattern != currentPattern) {
      break;
    }

  }
}