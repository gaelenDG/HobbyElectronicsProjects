#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define LED_PIN 2
#define NUM_LEDS 64
#define BUTTON_PIN 0 // GPIO 0 for the button
#define LED_BUILTIN 10 // Built-in LED pin
#define MQTT_TOPIC "home/OvenLEDs/brightness" // MQTT topic for brightness control
#define MQTT_TOPIC_STATE "home/OvenLEDs/state" // MQTT topic for state control
#define MQTT_TOPIC_STATE_RESPONSE "home/OvenLEDs/state_response" // MQTT topic for state response

// WiFi and MQTT credentials
const char* ssid = "WendellsWorld";
const char* password = "partytime";
const char* mqttServer = "192.168.0.200";
const int mqttPort = 1883;
const char* mqttUser = "mqtt_user";
const char* mqttPassword = "mqtt_password";
volatile bool buttonPressed = false; // Flag to indicate a button press

// Global variables
int brightness = 100;
bool lightOn = false; // Track the on/off state of the light
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50; // Debounce delay in milliseconds
bool lastButtonState = LOW; // Assume button is not pressed initially


// Function prototypes
void connectToWiFi();
void mqttCallback(char* topic, byte* message, unsigned int length);
void setBrightness(int newBrightness);
void setAllPixels(uint32_t color);
void handleButtonPress();

void IRAM_ATTR buttonISR() {
  // Check if the button is pressed (HIGH in your case)
  if (digitalRead(BUTTON_PIN) == HIGH) {
    buttonPressed = true; // Set the flag when the button is pressed
  }
}

Adafruit_NeoPixel LightStrip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel BUILTIN(1, LED_BUILTIN, NEO_GRB + NEO_KHZ800);

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Setup function
void setup() {
  BUILTIN.begin();
  BUILTIN.setPixelColor(0, BUILTIN.Color(10, 10, 0)); // Dim yellow light for booting feedback
  BUILTIN.show();

  // Serial monitor for debugging
  Serial.begin(115200);
  Serial.println("Starting up...");

  // Initialize NeoPixel strip
  LightStrip.begin();
  LightStrip.setBrightness(brightness);
  setAllPixels(LightStrip.Color(255, 225, 50)); // Set all pixels to whiteish yellow
  LightStrip.show();

  // Initialize button
  pinMode(BUTTON_PIN, INPUT);  // No need for PULLUP with the capacitive button
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, CHANGE); // Attach interrupt

  // Connect to WiFi & MQTT
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi");


  BUILTIN.setPixelColor(0, BUILTIN.Color(0, 10, 10)); // Green light for ready
  BUILTIN.show();

  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(mqttCallback);
  if (mqttClient.connect("ESP32C3_NeoPixel", mqttUser, mqttPassword)) {
    Serial.println("Connected to MQTT");
    mqttClient.subscribe(MQTT_TOPIC);
    mqttClient.subscribe(MQTT_TOPIC_STATE);
  } else {
    Serial.print("Failed to connect to MQTT, state: ");
    Serial.println(mqttClient.state());
  }

  BUILTIN.setPixelColor(0, BUILTIN.Color(0, 10, 0)); // Green light for ready
  BUILTIN.show();
  Serial.println("Setup complete");
}

// Main loop
void loop() {
  // Handle button press if the interrupt flag is set
  if (buttonPressed) {
    handleButtonPress();
  }

  // Process MQTT client loop to handle incoming messages
  mqttClient.loop();
}

void mqttCallback(char* topic, byte* message, unsigned int length) {
  // Convert the incoming message to a string
  char messageBuffer[length + 1];
  strncpy(messageBuffer, (char*)message, length);
  messageBuffer[length] = '\0';

  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(messageBuffer);

  // Handle brightness control
  if (strcmp(topic, MQTT_TOPIC) == 0) {
    int newBrightness = atoi(messageBuffer); // Convert message to integer
    setBrightness(newBrightness);
  }

  // Handle light state control
  if (strcmp(topic, MQTT_TOPIC_STATE) == 0) {
    if (strcmp(messageBuffer, "ON") == 0) {
      lightOn = true;
      setBrightness(brightness); // Update light state
    } else if (strcmp(messageBuffer, "OFF") == 0) {
      lightOn = false;
      setBrightness(brightness); // Update light state
    }
  }
}

void setBrightness(int newBrightness) {
  brightness = constrain(newBrightness, 0, 255);

  if (newBrightness <= 3) {
    // If brightness is at 1%, turn off all LEDs
    LightStrip.clear(); // Clear all pixels
  } else {
    // Otherwise, set the brightness and apply the default color
    LightStrip.setBrightness(brightness);
    if (lightOn) {
      setAllPixels(LightStrip.Color(255, 225, 50)); // Use the warm incandescent-like yellow
    }
  }

  LightStrip.show();

  // Publish the current brightness state
  char brightnessStr[4];
  itoa(brightness, brightnessStr, 10);
  mqttClient.publish("home/leds/current_brightness", brightnessStr);

  // Publish the current light state
  mqttClient.publish("home/CounterLEDs/state_response", lightOn ? "ON" : "OFF");
}

void setAllPixels(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    LightStrip.setPixelColor(i, color);
  }
  LightStrip.show();
}

void handleButtonPress() {
  Serial.println("Handling button press...");
  static unsigned long lastPressTime = 0; // Last time the button was pressed

  if (buttonPressed) {
    unsigned long currentTime = millis();
    if (currentTime - lastPressTime > debounceDelay) { // Debounce logic
      lightOn = !lightOn; // Toggle the light state
      Serial.print("Button pressed, toggling light state: ");
      Serial.println(lightOn ? "ON" : "OFF");

      // Update the light state
      if (lightOn) {
        // Turn on the NeoPixel array at the current brightness
        LightStrip.setBrightness(brightness);
        setAllPixels(LightStrip.Color(255, 225, 50)); // Set all pixels to whiteish yellow
      } else {
        // Turn off the NeoPixel array
        LightStrip.clear(); // Clear all pixels
      }
      LightStrip.show();

      // Debugging: Print the light state to the serial monitor
      Serial.print("Light state toggled: ");
      Serial.println(lightOn ? "ON" : "OFF");

      // Update the last press time
      lastPressTime = currentTime;
    }
    buttonPressed = false; // Reset the button pressed flag
  }
}