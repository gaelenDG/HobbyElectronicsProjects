// #include <Adafruit_NeoPixel.h>
// #include <vector>
// #include <iostream>
// #include <queue>
// #include <unordered_map>
// #include <esp_task_wdt.h>
// using namespace std;

// // NeoPixel setup
// #define NEOPIXEL_PIN1 6       // GPIO pin for first NeoPixel chain
// #define NEOPIXEL_PIN2 7       // GPIO pin for second NeoPixel chain
// #define NUM_PIXELS 12         // Number of NeoPixels per chain
// #define LED_BUILTIN 10

// Adafruit_NeoPixel pixels1(NUM_PIXELS, NEOPIXEL_PIN1, NEO_GRB + NEO_KHZ800);
// Adafruit_NeoPixel pixels2(NUM_PIXELS, NEOPIXEL_PIN2, NEO_GRB + NEO_KHZ800);
// Adafruit_NeoPixel BUILTIN(1, LED_BUILTIN, NEO_GRB + NEO_KHZ800);

// // Photoresistor setup
// #define PHOTORESISTOR_COUNT 5
// const int photoResistorPins[PHOTORESISTOR_COUNT] = {0, 1, 2, 3, 4};

// // Thresholds for detecting significant light changes
// int lightThresholds[PHOTORESISTOR_COUNT];
// const int buffer = 50; // Sensitivity buffer for activation

// // Declare the graph globally so it is accessible everywhere
// vector<int> graph[24] = {
//   {1, 5, 6, 7},
//   {0, 2, 3, 4, 5},
//   {1, 3, 14},
//   {1, 2, 4, 11},
//   {1, 3, 5, 9, 10, 11},
//   {0, 1, 4, 6, 9},
//   {0, 5, 7, 8, 9},
//   {0, 6, 8},
//   {6, 7, 9},
//   {4, 5, 6, 8, 21},
//   {4, 9, 11, 22},
//   {3, 4, 10, 23},
//   {13, 17, 18, 19},
//   {12, 14, 15, 16, 17},
//   {2, 13, 15, 16},
//   {13, 14, 16, 23},
//   {13, 15, 17, 21, 22, 23},
//   {12, 13, 16, 18, 21},
//   {12, 17, 19, 20, 21},
//   {12, 18, 20},
//   {9, 18, 19, 21},
//   {10, 16, 17, 18, 20},
//   {11, 16, 21, 23},
//   {15, 16, 22}
// };

// // Function prototypes
// void setLEDs(Adafruit_NeoPixel &pixels, const uint32_t colors[]);
// void calibrateThresholds();
// int determinePage();
// void printPhotoResistorValues(int activeCount);
// void lightPixel(int position, const uint32_t color[]);
// void lightRainbowPixelStep(int position, int step);
// void lightRainbowPixelStep2(int position, int step, int speed);
// void lightRainbowPixel3(int position, int speed);
// void multipleRandomWalkWithRainbow(int currentPattern);

// // Pattern function prototypes
// void multipleRandomWalk(int currentPattern);

// void patternSequential(Adafruit_NeoPixel &pixels, int currentPattern);
// void patternSequential2(int currentPattern);
// void patternStaticColors(Adafruit_NeoPixel &pixels, const uint32_t colors[]);
// void patternRandomWalk(Adafruit_NeoPixel &pixels, Adafruit_NeoPixel &pixels2, int currentPattern);
// void patternRandomWalk2(int currentPattern);
// void patternSpreadingWave(Adafruit_NeoPixel &pixels1, Adafruit_NeoPixel &pixels2, int currentPattern);
// void predatorPreyPattern(int currentPattern);
// void patternRainbowRandomWalk(int currentPattern); 

// void setup() {
//   // Initialize NeoPixels
//   pixels1.begin();
//   pixels2.begin();
//   BUILTIN.begin();

//   pixels1.setBrightness(25);
//   pixels2.setBrightness(25);
//   BUILTIN.setPixelColor(0, BUILTIN.Color(0, 255, 0));
//   pixels1.show();
//   pixels2.show();
//   BUILTIN.show();

//   // Initialize photoresistors
//   for (int i = 0; i < PHOTORESISTOR_COUNT; i++) {
//     pinMode(photoResistorPins[i], INPUT);
//   }

//   Serial.begin(9600);
//   Serial.println("System initializing...");

//   calibrateThresholds();
//   Serial.println("Threshold calibration complete. System ready.");

//   // Initialize watchdog
//   esp_task_wdt_init(10, true); // 5-second timeout (currently djsabled!)
//   esp_task_wdt_add(NULL); // Add current task to watchdog

// }

// void loop() {
//   int activePattern = determinePage();
//   static int lastPattern = -1;

//   esp_task_wdt_reset(); // Reset watchdog timer in each loop iteration

//   // Only update if the pattern changes
//   if (activePattern != lastPattern) {
//     lastPattern = activePattern;
//     Serial.print("Page open: ");
//     Serial.println(activePattern);

//     // Execute the corresponding pattern
//     switch (activePattern) {
//       case 0:
//         multipleRandomWalk(0);

//       case 1:
// 				patternRandomWalk(pixels1, pixels2, 2);

//       case 2: {
//         patternRainbowRandomWalk(1);
//       }
//       case 3: {
//         patternSpreadingWave(pixels1, pixels2, 3);
//       }
//       case 4: {
//         predatorPreyPattern(4);
//       }
//       case 5: {
//         multipleRandomWalkWithRainbow(5);
//       }
//     }
//   }
// // Continuously check for photoresistor changes
//   printPhotoResistorValues(activePattern);
// }


// // Calibrate light thresholds for each photoresistor
// void calibrateThresholds() {
//   for (int i = 0; i < PHOTORESISTOR_COUNT; i++) {
//     int rawValue = analogRead(photoResistorPins[i]);
//     lightThresholds[i] = 100;
//     Serial.print("Photoresistor ");
//     Serial.print(i);
//     Serial.print(" threshold: ");
//     Serial.println(lightThresholds[i]);
//   }
// }



// // Determine which pattern to display based on photoresistor activation
// int determinePage() {
//     int activeCount = 0;
//     for (int i = 0; i < PHOTORESISTOR_COUNT; i++) {
//         int rawValue = analogRead(photoResistorPins[i]);
//         if (rawValue > lightThresholds[i]) {
//             activeCount++;
//         }
//     }
//     esp_task_wdt_reset();
//     return activeCount; // Assuming activeCount maps to a pattern
// }


// // Print photoresistor values in a single line if exceeding thresholds
// void printPhotoResistorValues(int activeCount) {
//   static int lastActiveCount = -1;

//   // Only print when activeCount changes
//   if (activeCount != lastActiveCount) {
//     lastActiveCount = activeCount;
//     Serial.print("Photoresistor values: ");
//     for (int i = 0; i < PHOTORESISTOR_COUNT; i++) {
//       int rawValue = analogRead(photoResistorPins[i]);
//       Serial.print(rawValue);
//       if (i < PHOTORESISTOR_COUNT - 1) Serial.print(", ");
//     }
//     Serial.println();
//   }
// }

// // Light up a specified pixel with a specified color
// void lightPixel(int position, uint32_t color) {
//   // Determine which chain and pixel index to light up
//   Adafruit_NeoPixel* chain = nullptr;  // Pointer to the correct chain
//   int localPosition = 0;               // Pixel index within the selected chain

//   if (position >= 0 && position < NUM_PIXELS) {
//     chain = &pixels1;  // First chain
//     localPosition = position;
//   } else if (position >= NUM_PIXELS && position < 2 * NUM_PIXELS) {
//     chain = &pixels2;  // Second chain
//     localPosition = position - NUM_PIXELS;
//   }

//   if (chain) {  // If a valid chain is selected
//     chain->setPixelColor(localPosition, color);
//     chain->show();
//   } else {
//     // Handle invalid positions (optional)
//     Serial.print("Invalid position: ");
//     Serial.println(position);
//   }
// }

// void lightRainbowPixelStep(int position, int step) {
//     // Determine which chain and pixel index to light up
//     Adafruit_NeoPixel* chain = nullptr;
//     int localPosition = 0;

//     if (position >= 0 && position < NUM_PIXELS) {
//         chain = &pixels1;
//         localPosition = position;
//     } else if (position >= NUM_PIXELS && position < 2 * NUM_PIXELS) {
//         chain = &pixels2;
//         localPosition = position - NUM_PIXELS;
//     }

//     if (!chain) {
//         Serial.print("Invalid position: ");
//         Serial.println(position);
//         return;
//     }

//     // Calculate colors based on the step
//     int red = max(0, 255 - step);
//     int green = max(0, step - 255);
//     int blue = max(0, 510 - step);

//     uint32_t color = chain->Color(red, green, blue);
//     chain->setPixelColor(localPosition, color);
//     chain->show();
// }

// void lightRainbowPixelStep2(int position, int speed) {
//   static unsigned long lastUpdateTime = 0;
//   static int step = 0;

//   unsigned long currentTime = millis();
  
//   // Only update the color if enough time has passed
//   if (currentTime - lastUpdateTime >= (speed / 510)) {
//       lastUpdateTime = currentTime;  // Update time
      

//       // Determine which chain and pixel index to light up
//       Adafruit_NeoPixel* chain = nullptr;
//       int localPosition = 0;

//       if (position >= 0 && position < NUM_PIXELS) {
//           chain = &pixels1;
//           localPosition = position;
//       } else if (position >= NUM_PIXELS && position < 2 * NUM_PIXELS) {
//           chain = &pixels2;
//           localPosition = position - NUM_PIXELS;
//       } else {
//           Serial.print("Invalid position: ");
//           Serial.println(position);
//           return;
//       }

//       // Compute smooth rainbow colors
//       int red = max(0, 255 - step);
//       int green = max(0, step - 255);
//       int blue = max(0, 510 - step);

//       // Apply the color
//       uint32_t color = chain->Color(red, green, blue);
//       chain->setPixelColor(localPosition, color);
//       chain->show();

//       // Advance the step, looping back to zero after a full cycle
//       step = (step + 1) % 510;
//   }
// }

// void lightRainbowPixel3(int position, int speed) {
//     // Determine which chain and pixel index to light up
//     Adafruit_NeoPixel* chain = nullptr;
//     int localPosition = 0;

//     if (position >= 0 && position < NUM_PIXELS) {
//         chain = &pixels1;
//         localPosition = position;
//     } else if (position >= NUM_PIXELS && position < 2 * NUM_PIXELS) {
//         chain = &pixels2;
//         localPosition = position - NUM_PIXELS;
//     }

//     if (!chain) {
//         Serial.print("Invalid position: ");
//         Serial.println(position);
//         return;
//     }

//     // Calculate the current time-based step for smooth cycling
//     unsigned long currentTime = millis();
//     int totalStep = (currentTime % speed) * 765 / speed; // Maps time into a 0-765 range

//     // Calculate the color based on the step
//     int red, green, blue;
//     if (totalStep < 255) { // Red to Green
//         red = 255 - totalStep;
//         green = totalStep;
//         blue = 0;
//     } else if (totalStep < 510) { // Green to Blue
//         totalStep -= 255;
//         red = 0;
//         green = 255 - totalStep;
//         blue = totalStep;
//     } else { // Blue to Red
//         totalStep -= 510;
//         red = totalStep;
//         green = 0;
//         blue = 255 - totalStep;
//     }

//     // Set the pixel color
//     uint32_t color = chain->Color(red, green, blue);
//     chain->setPixelColor(localPosition, color);
//     chain->show();
// }


// // Set NeoPixels to a specific color pattern
// void patternStaticColors(Adafruit_NeoPixel &pixels, const uint32_t colors[]) {
//   for (int i = 0; i < NUM_PIXELS; i++) {
//     pixels.setPixelColor(i, colors[i]);
//   }
//   pixels.show();  // Update NeoPixel strip
// }

// // Pattern: Sequential lighting
// void patternSequential(Adafruit_NeoPixel &pixels, int currentPattern) {
//     for (int i = 0; i < NUM_PIXELS; i++) {
//         if (determinePage() != currentPattern) {
//             return; // Exit if pattern changes
//         }
//         pixels.clear();
//         pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Green
//         pixels.show();
//         delay(500);
//     }
// }

// // Pattern: Sequential lighting
// void patternSequential2(int currentPattern) {

//   // initialize the current starting pattern
//   int activePattern = determinePage();

//   int rainbowStep = 0;  // Step counter for the rainbow cycle

//   while(activePattern == currentPattern){
//     for (int i = 0; i < NUM_PIXELS * 2; i ++) {

//       // start each cycle with all lights off
//       pixels1.clear();
//       pixels2.clear();
//       pixels1.show();
//       pixels2.show();

      
//       Adafruit_NeoPixel* chain = nullptr;  // Pointer to the correct chain

      
//       lightRainbowPixelStep(i, 100); 
//       rainbowStep += 10;  // Adjust step increment for desired transition speed
      
      
//       int activePattern = determinePage();

//       if (determinePage() != currentPattern) {
//             return; // Exit if pattern changes
//         }

//       delay(1000);
//     }
//   }
// }

// // Pattern: Random walk
// void patternRandomWalk(Adafruit_NeoPixel &pixels1, Adafruit_NeoPixel &pixels2, int currentPattern) {

//   // Starting node will always be position 0
//   static int node = 0;

//   // Monitor for whether the current page is correct
//   int activePattern = determinePage();

//   // Start page behavior with all pixels off
//   pixels1.clear();
//   pixels2.clear();
//   pixels1.show();
//   pixels2.show();

//   // Define the color of the NeoPixel to travel across the page
//   uint32_t color = pixels1.Color(50, 25, 25);

//   // While loop determines if the correct page is still open, keeps the pattern going if true
//   while (activePattern == currentPattern) {

//     // Determine which light on which chain is the proper one to illuminate for the current node
//     if (node >= 0 && node < NUM_PIXELS) {
//       // Chain1: Nodes 0-(NUM_PIXELS-1)
//       pixels1.setPixelColor(node, color);
//       pixels1.show();
//     } else if (node >= NUM_PIXELS && node < 2 * NUM_PIXELS) {
//       // Chain2: Nodes NUM_PIXELS-(2*NUM_PIXELS-1)
//       pixels2.setPixelColor(node - NUM_PIXELS, color);
//       pixels2.show();
//     }

//     // Keeps the light on for 0.5 sec before moving on
//     delay(500);

//     // Find a new node neighbor to go to
//     int nextNode = graph[node][random(0, graph[node].size())];

//     // Update the serial monitor with the move to be made, to help determine if things are working properly
//     Serial.print("Moving from ");
//     Serial.print(node);
//     Serial.print(" to ");
//     Serial.println(nextNode);
//     node = nextNode;

//     // Turn all the pixels off to prepare for the next iteration
//     pixels1.clear();
//     pixels2.clear();
//     pixels1.show();
//     pixels2.show();

//     // Determine which page is open, update the while loop
//     activePattern = determinePage();
//   }
// }

// // Pattern: Improved random walk
// void patternRandomWalk2(int currentPattern){

//   // Starting node will be random
//   static int node = random(0, 2 * NUM_PIXELS);

//   // Monitor for whether the current page is correct
//   int activePattern = determinePage();

//   // Start page behavior with all pixels off
//   pixels1.clear();
//   pixels2.clear();
//   pixels1.show();
//   pixels2.show();

//   // Define the color of the NeoPixel to travel across the page
//   Adafruit_NeoPixel* chain = nullptr;  // Pointer to the correct chain
//   uint32_t color = chain->Color(20, 15, 0);
  
//   // While loop determines if the correct page is still open, keeps the pattern going if true
//   while (activePattern == currentPattern) {

//     // Determine which page is open, update the while loop
//     int activePattern = determinePage();
//     if (activePattern != currentPattern) {
//       break;
//     }

//     // Apply light to the pattern
//     lightPixel(node, color);

//     // Pause for light
//     delay(500);

//     // Find a new node neighbor to go to
//     int nextNode = graph[node][random(0, graph[node].size())];

//     // Update the serial monitor with the move to be made, to help determine if things are working properly
//     Serial.print("Moving from ");
//     Serial.print(node);
//     Serial.print(" to ");
//     Serial.println(nextNode);
//     node = nextNode;

//     // Turn all the pixels off to prepare for the next iteration
//     pixels1.clear();
//     pixels2.clear();
//     pixels1.show();
//     pixels2.show();
//   }

// }

// // Pattern: Spreading wave
// void patternSpreadingWave(Adafruit_NeoPixel &pixels1, Adafruit_NeoPixel &pixels2, int currentPattern) {

//   // Monitor for whether the correct page is open
//   int activePattern = determinePage();


//   // Clear all pixels at the start of a new wave
//   pixels1.clear();
//   pixels2.clear();
//   pixels1.show();
//   pixels2.show();

//   // While loop keeps the pattern going as long as the correct page is open
//   while (activePattern == currentPattern) {

//     // Choose a random starting node
//     int startNode = random(0, 2 * NUM_PIXELS);

//     // Generate a random color for the wave
//     uint32_t color = pixels1.Color(random(0, 128), random(0, 128), random(0, 128));

//     // Array to track visited nodes
//     bool visited[2 * NUM_PIXELS] = {false};

//     // Simple queue implementation using an array
//     int queue[2 * NUM_PIXELS]; // Maximum possible size is the total number of nodes
//     int front = 0, back = 0;

//     // Enqueue the starting node
//     queue[back++] = startNode;
//     visited[startNode] = true;

//     // Begin the wave propagation
//     while (front < back && activePattern == currentPattern) {
//       int currentNode = queue[front++]; // Dequeue the current node

//       // Determine which chain and pixel index to light up
//       if (currentNode >= 0 && currentNode < NUM_PIXELS) {
//         pixels1.setPixelColor(currentNode, color);
//         pixels1.show();
//       } else if (currentNode >= NUM_PIXELS && currentNode < 2 * NUM_PIXELS) {
//         pixels2.setPixelColor(currentNode - NUM_PIXELS, color);
//         pixels2.show();
//       }

//       // Add a small delay for the wave effect
//       delay(200);

//       // Enqueue all unvisited neighbors
//       for (int neighbor : graph[currentNode]) {
//         if (!visited[neighbor]) {
//           queue[back++] = neighbor;
//           visited[neighbor] = true;
//         }
//       }

//       // Determine which page is open, update the while loop
//       activePattern = determinePage();
//     }

//     // Delay for a moment before restarting the wave with a new random color
//     delay(1000);

//     // Determine which page is open, update the while loop
//     activePattern = determinePage();
//     if (activePattern != currentPattern) {
//       break;
//     }
//   }
// }

// void predatorPreyPattern(int currentPattern) {

//   // determine the active page
//   int activePattern = determinePage();
//   int iterationCount = 0;

//   // Assign a random starting position for each of the two active nodes
//   int fleeingNodePosition = random(0, 2 * NUM_PIXELS);
//   int chasingNodePosition = random(0, 2 * NUM_PIXELS);


//   uint32_t fleeingNodeColor = pixels1.Color(0, 0, 50);
//   uint32_t chasingNodeColor = pixels2.Color(50, 0, 0);

//   // Light the two nodes with the appropriate color
//   lightPixel(fleeingNodePosition, fleeingNodeColor);
//   lightPixel(chasingNodePosition, chasingNodeColor);

//   // Delay before starting the loop
//   delay(500);

//   while (activePattern == currentPattern) {
//     // Clear all pixels at the start of each frame
//     pixels1.clear();
//     pixels2.clear();
//     pixels1.show();
//     pixels2.show();

//     // helps limit memory leak by breaking loop if the number of iterations goes too high
//     if (++iterationCount > 1000) { // Arbitrary large number
//         Serial.println("Pattern iteration limit reached. Exiting.");

//         return;
//     }

//     // FLEEING NODE: Move to a random neighbor
//     int nextNode = graph[fleeingNodePosition][random(0, graph[fleeingNodePosition].size())];
//     fleeingNodePosition = nextNode;

//     // Some stuff to help keep the pattern from getting stuck

//     if (random(0, 10) == 0) { // 10% chance to randomly relocate
//       fleeingNodePosition = random(0, 2 * NUM_PIXELS);
//       Serial.println("Fleeing node relocated to avoid loops!");
//     }   


//     // Light the fleeing node
//     lightPixel(fleeingNodePosition, fleeingNodeColor);

//     // Delay for visualization
//     delay(250);

//     // CHASING NODE: Find the next step on the shortest path
//     std::queue<int> queue;
//     std::unordered_map<int, int> cameFrom; // Tracks the path back to the start

//     queue.push(chasingNodePosition);
//     cameFrom[chasingNodePosition] = -1; // Start node has no parent

//     while (!queue.empty()) {
//     int current = queue.front();
//     queue.pop();

//     if (current == fleeingNodePosition) {
//         break; // Path found
//     }

//     for (int neighbor : graph[current]) {
//         if (cameFrom.find(neighbor) == cameFrom.end()) {
//             queue.push(neighbor);
//             cameFrom[neighbor] = current;
//         }
//     }
//     }

//     // If queue is empty and fleeingNodePosition is not reached:
//     if (cameFrom.find(fleeingNodePosition) == cameFrom.end()) {
//         // Handle the case where no path is found
//         Serial.println("No path found to fleeing node!");
//         return; // Exit the function to avoid freezing
//     }

//     // Determine the next step toward the fleeing node
//     int step = fleeingNodePosition;
//     while (cameFrom[step] != chasingNodePosition) {
//       step = cameFrom[step];
//     }
//     chasingNodePosition = step;

//     // Light the chasing node
//     lightPixel(chasingNodePosition, chasingNodeColor);

//     // Check if the chasing node caught the fleeing node
//     if (fleeingNodePosition == chasingNodePosition) {
//       // Combine colors
//       uint8_t r1 = (fleeingNodeColor >> 16) & 0xFF;
//       uint8_t g1 = (fleeingNodeColor >> 8) & 0xFF;
//       uint8_t b1 = fleeingNodeColor & 0xFF;

//       uint8_t r2 = (chasingNodeColor >> 16) & 0xFF;
//       uint8_t g2 = (chasingNodeColor >> 8) & 0xFF;
//       uint8_t b2 = chasingNodeColor & 0xFF;

//       uint8_t rSum = (r1 + r2) % 256;
//       uint8_t gSum = (g1 + g2) % 256;
//       uint8_t bSum = (b1 + b2) % 256;

//       chasingNodeColor = pixels1.Color(rSum, gSum, bSum);

//       // Generate a new fleeing node
//       fleeingNodePosition = random(0, 2 * NUM_PIXELS);
//       fleeingNodeColor = pixels1.Color(random(0, 50), random(0, 50), random(50));
//     }

//     // Delay for visualization
//     delay(250);

//     // do another check for if the page is right
//     int activePattern = determinePage();
//   }
// }

// void patternRainbowRandomWalk(int currentPattern) {
//     // Determine the active page
//     int activePattern = determinePage();

//     // Randomly assign a starting node
//     int rainbowNodePosition = random(0, 2 * NUM_PIXELS);

//     int rainbowStep = 0;  // Step counter for the rainbow cycle

//     while (activePattern == currentPattern) {
//         // Clear all pixels at the start of each frame
//         pixels1.clear();
//         pixels2.clear();
//         pixels1.show();
//         pixels2.show();

//         // Update the rainbow color for the current node
//         lightRainbowPixelStep(rainbowNodePosition, rainbowStep);
//         rainbowStep += 10;  // Adjust step increment for desired transition speed

//         // If the rainbow cycle is complete (step > 510), move to a new node
//         if (rainbowStep > 510) {
//             rainbowStep = 0;  // Reset the rainbow cycle
//             int nextNode = graph[rainbowNodePosition][random(0, graph[rainbowNodePosition].size())];
//             rainbowNodePosition = nextNode;
//         }

//         // Check for the active page (to allow pattern switching)
//         activePattern = determinePage();

//         // Delay to control update speed
//         delay(50);
//     }
// }

// void multipleRandomWalk(int currentPattern) {
//   // Firefly color
//   uint32_t FireflyColor = pixels2.Color(10, 10, 0);

//   // Number of lights in the pattern
//   const int numLights = 6;

//   // Total number of nodes (adjust to match your graph size)
//   const int numNodes = 24;

//   // Current positions of the lights
//   int positions[numLights];

//   // Occupied state for each node
//   bool occupied[numNodes] = {false};

//   // Initialize the positions of the lights at random, ensuring no collisions
//   for (int i = 0; i < numLights; i++) {
//       int randomPosition;
//       do {
//           randomPosition = random(0, numNodes);
//       } while (occupied[randomPosition]);
//       positions[i] = randomPosition;
//       occupied[randomPosition] = true;
//   }
//   // An "off" color pattern
//   uint32_t LightsOff = pixels2.Color(0, 0, 0);

//   // Initialize active page 
//   int activePattern = determinePage();

//   // Loop until the page changes
//   while (determinePage() == activePattern) {

//     int activePattern = determinePage();
//     // Clear all pixels before updating positions
//     for (int i = 0; i < numNodes; i++) {
//         lightPixel(i, LightsOff); // Turn off all pixels
//     }

//     // Move each light to a random neighbor
//     for (int i = 0; i < numLights; i++) {
//       int currentPosition = positions[i];
//       int nextPosition;

//       // Find an unoccupied neighbor
//       bool moved = false;
//       for (int attempts = 0; attempts < graph[currentPosition].size(); attempts++) {
//         int neighborIndex = random(0, graph[currentPosition].size());
//         nextPosition = graph[currentPosition][neighborIndex];
//         if (!occupied[nextPosition]) {
//           occupied[currentPosition] = false;
//           positions[i] = nextPosition;
//           occupied[nextPosition] = true;
//           moved = true;
//           break;
//           }
//       }

//       // If no move was possible, stay in place
//       if (!moved) {
//           nextPosition = currentPosition;
//       }

//       // Light up the current position
//       lightPixel(nextPosition, FireflyColor);
//     }

//     // Small delay for smoother animation
//     delay(500);
//   }

//   // Clear lights when exiting the pattern
//   for (int i = 0; i < numNodes; i++) {
//     lightPixel(i, LightsOff); // Turn off all pixels
//   }
// }

// void multipleRandomWalkWithRainbow(int currentPattern) {
//   // Firefly color
//   uint32_t FireflyColor = pixels2.Color(10, 10, 0);

//   // Number of lights in the pattern
//   const int numLights = 6;
//   const int numNodes = 24;

//   // Current positions of the lights
//   int positions[numLights];
//   bool occupied[numNodes] = {false};

//   // Initialize positions with no collisions
//   for (int i = 0; i < numLights; i++) {
//       int randomPosition;
//       do {
//           randomPosition = random(0, numNodes);
//       } while (occupied[randomPosition]);
//       positions[i] = randomPosition;
//       occupied[randomPosition] = true;
//   }

//   // Select one light as the rainbow
//   int rainbowIndex = random(0, numLights);

//   uint32_t LightsOff = pixels2.Color(0, 0, 0);
//   int activePattern = determinePage();

//   while (activePattern == currentPattern) {
//     // Clear all pixels before updating positions
//     for (int i = 0; i < numNodes; i++) {
//         lightPixel(i, LightsOff);
//     }

//     // Move each light to a new position
//     for (int i = 0; i < numLights; i++) {
//         int currentPosition = positions[i];
//         int nextPosition = currentPosition;

//         // Try to move to an unoccupied neighbor
//         bool moved = false;
//         for (int attempts = 0; attempts < graph[currentPosition].size(); attempts++) {
//             int neighborIndex = random(0, graph[currentPosition].size());
//             nextPosition = graph[currentPosition][neighborIndex];

//             if (!occupied[nextPosition]) {
//                 occupied[currentPosition] = false;
//                 positions[i] = nextPosition;
//                 occupied[nextPosition] = true;
//                 moved = true;
//                 break;
//             }
//         }

//         // Light up the current position
//         if (i == rainbowIndex) {
//             lightRainbowPixelStep2(nextPosition, 250);
//         } else {
//             lightPixel(nextPosition, FireflyColor);
//         }
//     }

//     delay(500);  // Shorter delay for smoother transitions

//     // Initialize active page 
//     int activePattern = determinePage();
//   }

//   // Turn off all lights upon exit
//   for (int i = 0; i < numNodes; i++) {
//       lightPixel(i, LightsOff);
//   }
// }
