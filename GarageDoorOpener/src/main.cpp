#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include "esp_sleep.h"

// GPIO pins
#define DoorTriggerPin 0
#define BuiltIn_NeoPixel 10
#define HallSensorPin 5

// Global variables
String DoorState = "closed"; // Flag to indicate if the door is closed
const unsigned long debounceDelay = 50; // Debounce delay in milliseconds
unsigned long doorOpenTimestamp = 0; // Timestamp when the door was last detected as open
unsigned long autoCloseDelay = 10 * 60 * 1000; // 10 minutes in milliseconds
String logMsg;

// Status light
Adafruit_NeoPixel StatusLight(1, BuiltIn_NeoPixel, NEO_GRB + NEO_KHZ800);

// Wi-Fi credentials
const char* ssid = "WendellsWorld";
const char* password = "partytime";

// MQTT broker credentials
const char* mqttServer = "192.168.0.200";
const char* mqttUser = "myuser";
const char* mqttPassword = "mqttpassword";
const int mqttPort = 1883;

// MQTT topics
const char* command_topic = "esp32/GarageDoor/Toggle";
const char* MQTT_TOPIC_STATE = "esp32/HallSensorGarage/DoorState";
const char* MQTT_TIMER = "esp32/GarageDoor/Timer";

WiFiClient espClient;
PubSubClient mqttClient(espClient);


// Function prototypes
void connectToWiFi();
void connectToMQTT();
void mqttCallback(char* topic, byte* message, unsigned int length);
void ToggleGarageDoor();
void checkDoorState();
void checkDoorOpenTimestamp();
void mqttLog(const String& message);


void setup() {

  // Serial monitor for debugging
  Serial.begin(115200);
  logMsg = "Starting up...";
  Serial.println(logMsg);
  mqttLog(logMsg);


  // Initialize status light
  StatusLight.begin();
  StatusLight.setPixelColor(0, StatusLight.Color(10, 10, 10)); // Dim white light for booting feedback
  StatusLight.show();

  // Initialize Hall Effect sensor pin
  pinMode(HallSensorPin, INPUT);

  // Set the initial state of the door
  checkDoorState();

  // Initialize door trigger pin
  pinMode(DoorTriggerPin, OUTPUT);
  digitalWrite(DoorTriggerPin, LOW); // Set door trigger pin to LOW (no signal)

  // Connect to WiFi & MQTT
  logMsg = "Connecting to WiFi...";
  Serial.print(logMsg);
  mqttLog(logMsg);
  connectToWiFi();

  logMsg = "Connecting to MQTT";
  Serial.println(logMsg);
  mqttLog(logMsg);
  connectToMQTT();

  // ...existing code in setup()...
  // Publish the current timer value to MQTT for Home Assistant feedback
  mqttClient.publish(MQTT_TIMER, String(autoCloseDelay / 60000).c_str(), true);
}

void loop() {

  // Check the door state periodically
  checkDoorState();

  // Check if the door has been open for too long
  if (DoorState == "open") {
    checkDoorOpenTimestamp();
  }

  // Process MQTT client loop to handle incoming messages
  mqttClient.loop();

  // Check if WiFi and MQTT are connected
  if (WiFi.status() != WL_CONNECTED) {
    logMsg = "WiFi disconnected. Reconnecting...";
    Serial.println(logMsg);
    mqttLog(logMsg);

    connectToWiFi();
  }
  if (!mqttClient.connected()) {
    logMsg = "MQTT disconnected. Reconnecting...";
    Serial.println(logMsg);
    mqttLog(logMsg);
    connectToMQTT();
  }
}


void connectToWiFi() {
  logMsg = "Connecting to Wi-Fi";
  Serial.print(logMsg);
  mqttLog(logMsg);

  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connected");
    mqttLog("Wi-Fi connected");
  } else {
    Serial.println("\nWi-Fi connection failed after 10 attempts.");
    mqttLog("Wi-Fi connection failed after 10 attempts.");
  }
}

void connectToMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  mqttClient.setCallback(mqttCallback);
  logMsg = "Connecting to MQTT broker...";
  Serial.print(logMsg);
  mqttLog(logMsg);

  int attempts = 0;
  while (!mqttClient.connected() && attempts < 10) {
    if (mqttClient.connect("ESP32C3_NeoPixel", mqttUser, mqttPassword)) {
      mqttClient.subscribe(command_topic);
      mqttClient.subscribe(MQTT_TOPIC_STATE);
      mqttClient.subscribe(MQTT_TIMER);
      Serial.println("connected"); // Report MQTT connection status
      mqttLog("MQTT connected");
      StatusLight.setPixelColor(0, StatusLight.Color(1, 0, 0)); // Dim green light for booting feedback
      StatusLight.show();
      break;
    } else {
      Serial.print(".");
      delay(500);
      attempts++;
    }
  }
  if (!mqttClient.connected()) {
    Serial.println("\nMQTT connection failed after 10 attempts.");
    mqttLog("MQTT connection failed after 10 attempts.");
  }
}

void ToggleGarageDoor() {
  
  // Reset timer to the default value, publish to MQTT
  autoCloseDelay = 10 * 60 * 1000; // Reset to 10 minutes
  mqttClient.publish(MQTT_TIMER, String(autoCloseDelay / 60000).c_str(), true); // Publish the new timer value

  
  // Change status light to red
  StatusLight.setPixelColor(0, StatusLight.Color(0, 100, 0)); // Bright red light
  StatusLight.show();
  logMsg = "Toggling garage door...";
  Serial.println(logMsg);
  mqttLog(logMsg);
  
  // Send a short HIGH pulse
  digitalWrite(DoorTriggerPin, HIGH); // Set door trigger pin to HIGH
  delay(100); // Wait for 100 milliseconds
  digitalWrite(DoorTriggerPin, LOW); // Set door trigger pin to LOW

  delay(15000); // Wait for 10 seconds to allow the door to respond
}

void mqttCallback(char* topic, byte* message, unsigned int length) {
  // Convert the incoming message to a string
  String messageTemp;
  for (unsigned int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }

  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  Serial.println(messageTemp);

  // Handle the timer topic
  if (String(topic) == MQTT_TIMER) {
    int newTimerValue = messageTemp.toInt(); // Convert the message to an integer
    if (newTimerValue > 0) {
      logMsg = "Updated auto-close timer to: " + String(newTimerValue) + " minutes.";
      Serial.print(logMsg);
      mqttLog(logMsg);
      // Publish back to MQTT to confirm the value (keeps HA in sync)
      mqttClient.publish(MQTT_TIMER, String(newTimerValue).c_str(), true);
    }
  }

  // Handle the command topic
  if (String(topic) == command_topic) {
    if (messageTemp == "CLOSE") {
      Serial.println("Closing garage door...");
      
      // Check the current state of the door
      if (DoorState == "open") {
        Serial.println("Door is open. Sending toggle command to close it...");
        ToggleGarageDoor(); // Send the toggle command to close the door
        DoorState = "closed"; // Update the state immediately
        mqttClient.publish(MQTT_TOPIC_STATE, "closed", true); // Publish the new state
      } else {
        Serial.println("Door is already closed. No action needed.");
      }
    } else if (messageTemp == "OPEN") {
      Serial.println("Opening garage door...");
      
      // Check the current state of the door
      if (DoorState == "closed") {
        Serial.println("Door is closed. Sending toggle command to open it...");
        ToggleGarageDoor(); // Send the toggle command to open the door
        DoorState = "open"; // Update the state immediately
        mqttClient.publish(MQTT_TOPIC_STATE, "open", true); // Publish the new state
      } else {
        Serial.println("Door is already open. No action needed.");
      }
    }
  }

  // Handle the state topic (if needed)
  if (String(topic) == MQTT_TOPIC_STATE) {
    Serial.print("Received state update: ");
    Serial.println(messageTemp);
    // Update DoorState or perform other actions based on the state
    DoorState = messageTemp; // Sync the DoorState variable with the received state
  }
}

void checkDoorState() {
  static String lastState = ""; // Store the last reported state
  static unsigned long lastDebounceTime = 0; // Timestamp for debounce
  const unsigned long debounceDelay = 50; // Debounce delay in milliseconds

  
  // Use the global DoorState variable as the current state
  String currentState = DoorState;

  // Read the Hall Effect sensor
  if (digitalRead(HallSensorPin) == LOW) {
    currentState = "closed";
  } else {
    currentState = "open";
  }

  // Publish the state only if it has changed
  if (currentState != lastState) {
    
    DoorState = currentState; // Update the global DoorState variable
    lastState = currentState; // Update the last state
    logMsg = "Door state changed to: " + currentState;
    Serial.print(logMsg);
    mqttLog(logMsg);

    // Publish the state to the MQTT topic
    mqttClient.publish(MQTT_TOPIC_STATE, currentState.c_str(), true); // Retain the message

    // Update the door open timestamp if the door is opened
    if (currentState == "open") {
      doorOpenTimestamp = millis();
      Serial.println("Door opened. Timer started.");

      StatusLight.setPixelColor(0, StatusLight.Color(0, 1, 0)); // Dim red light for open state
      StatusLight.show();

    } else {
      doorOpenTimestamp = 0; // Reset the timestamp if the door is closed
      Serial.println("Door closed. Timer reset.");

      StatusLight.setPixelColor(0, StatusLight.Color(0, 0, 0)); // Status light off for closed state
      StatusLight.show();
    }
  }
}


void checkDoorOpenTimestamp() {
  // Check if the door has been open for too long
  if (doorOpenTimestamp > 0 && millis() - doorOpenTimestamp >= autoCloseDelay) {
    logMsg = "Door has been open for too long. Sending CLOSE command...";
    Serial.println(logMsg);
    mqttLog(logMsg);
    ToggleGarageDoor();
    }
}

void mqttLog(const String& message) {
  mqttClient.publish("esp32/GarageDoor/Log", message.c_str());
}