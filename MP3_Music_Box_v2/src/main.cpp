// ======== Library initialization ========
#include <Arduino.h>
#include <DFRobotDFPlayerMini.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "esp32-hal-gpio.h"
#include "peripherals.h"
#include "config.h"
#include "helpers.h"

// using namespace std;


void setup() {

  // Init USB serial port for debugging
  Serial.begin(9600);

  // Initialize status light
  pinMode(StatusLight_R_Pin, OUTPUT);
  pinMode(StatusLight_G_Pin, OUTPUT);
  pinMode(StatusLight_B_Pin, OUTPUT);

  // Show booting up with dim yellow
  setStatusLight(10, 5, 0);
  
  // Connect to WiFi & MQTT
  logMsg = "Connecting to WiFi...";
  connectToWiFi();

  logMsg = "Connecting to MQTT";
  connectToMQTT();

  // Initialize buttons
  pinMode(Button1_pin, INPUT);
  pinMode(Button2_pin, INPUT);

  // Do a check of battery status on bootup
  checkBatteryAndSleepIfLow();

  // ----------- Initialize MP3 UART connection ------------------
  bool DFRobot_connected = false; // little check for the MP3 player connection

  MP3Serial.begin(9600, SERIAL_8N1, /*rx*/ 17, /*tx*/ 16); 
  delay(250);
  unsigned long t0 = millis();
  while (millis() - t0 < 2000) {
    if (MP3Serial.available()) Serial.printf("Got: 0x%02X\n", MP3Serial.read());
  }


  if (!player.begin(MP3Serial)) {
    Serial.println("❌ Connecting to DFPlayer Mini failed!");
    Serial.println("Check wiring, SD card, and voltage levels.");
    setStatusLight(0, 10, 10); 

    DFRobot_connected = false; 

  } 

  DFRobot_connected = true; 

  Serial.println("DFRobot player connected!");

  
  

  // ----------- Initialize NFC reader ------------------
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  bool NFCconnected = false;
  if (!versiondata) {
    Serial.println("Didn't find PN53x board");

    setStatusLight(10, 0, 10);

  } else {
    NFCconnected = true;
    Serial.println("PN532 connected!");
    nfc.SAMConfig();
  }

  
  // --------- Final check, update status light if ready--------- 
  checkPeripherals();
  if (DFRobot_connected && NFCconnected) {
    setStatusLight(0, 5, 0);  // green = all good

    // Play removed chime then stop playback
    player.volume(10);
    player.playFolder(01,001);

    // Instead of hard/blocking delay, soft yield
    unsigned long chimeStart = millis();
    while (millis() - chimeStart < 2000) {
      yield(); // feed OS/wdt so we won't panic
    }

    player.volume(volume);
    player.stop();
  }
  
}

void loop() {


  // Check battery levels, put to sleep if not charged sufficiently
  checkBatteryAndSleepIfLow();

  // Check for button presses, perform actions
  handleButtonAction();

  // Wait for a card
  boolean success;
  uint8_t uid[7];
  uint8_t uidLen = 0;
  static unsigned long lastCooldown = 0;

  // 
  if (millis() - lastNfcCheck >= nfcInterval) {

    checkPeripherals();
    
    lastNfcCheck = millis();

    Serial.println("Waiting for a tag... (tap now)");

    bool success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLen, 50);

    // If tag detected, build new string for comparison with global
    if (success) {

      // Change status light to show playing
      setStatusLight(0, 0, 5); 

      // update last seen time
      lastTagSeen = millis();

      // If new tag detected 

      bool newTag = !tagPresent || (uidLen != currentUIDLength) || (memcmp(uid, currentUID, uidLen) != 0);

      if (newTag) {

        player.volume(volume);
  
        tagPresent = true;
        memcpy(currentUID, uid, uidLen);
        currentUIDLength = uidLen;
        hasPlayedForCurrentTag = false;  // allow playback for this tag

        // Print UID as hex
        Serial.print("Current UID: ");
        for (uint8_t i = 0; i < currentUIDLength; i++) {
          if (currentUID[i] < 0x10) Serial.print("0"); // leading zero
          Serial.print(currentUID[i], HEX);
        }
        Serial.println();

      } 

      // Here could be where I map the records to their respective files...
      // For now, just pulling the folder from the payload and playing.
      // Attempt to read NDEF text payload
      // Triggers playback only once per tag reading
      if (!hasPlayedForCurrentTag) {
        String payload = readNdefTextFromTag();
        if (payload.length() == 0) {
          Serial.println("No NDEF text found (or read failed). Ensure tag is NDEF formatted and contains a Text record.");
        } else {
          Serial.print("NDEF text payload: '");
          Serial.print(payload);
          Serial.println("'");

          // Example mapping: payload contains "01/001" or "001" etc
          // You can adapt the mapping rule to your scheme.
          // If you use numeric ID "001" -> play /001.mp3
          // If you use folder format "01/001" -> use player.playFolder(1, 1)
          // Example below assumes payload like "001" or "01/001"
          if (payload.indexOf('/') >= 0) {
            int slash = payload.indexOf('/');
            int folder = payload.substring(0, slash).toInt();
            int file   = payload.substring(slash + 1).toInt();
            Serial.printf("Map to folder=%d, file=%d\n", folder, file);
            player.loopFolder(folder); // Send to DFPlayer
          } else {
            int file = payload.toInt();
            Serial.printf("Map to file=%d\n", file);
            player.play(file); // Send to DFPlayer
          }
        }
        hasPlayedForCurrentTag = true; // allow playback
      }

    } else {

      // No tag read this iteration: check timeout to decide removal
      if (tagPresent && millis() - lastTagSeen > TAG_TIMEOUT) {
        tagPresent = false;
        hasPlayedForCurrentTag = false;  // reset so next tag triggers playback
        Serial.println("Tag removed - stopping playback");

        // Change status light to show removed 
        setStatusLight(0, 5, 0); 

        // Play removed chime then stop playback
        player.volume(10);
        player.playFolder(01,002);
        
        // Instead of hard/blocking delay, soft yield
        unsigned long chimeStart = millis();
        while (millis() - chimeStart < 2500) {
          yield(); // feed OS/wdt so we won't panic
        }

        player.volume(volume);
        player.stop();

        // checkPeripherals();

        // Clear UID state
        currentUIDLength = 0;
        memset(currentUID, 0, sizeof(currentUID));
      }
    }
    
  }

  // Short delay to prevent busy-looping
  delay(10);
}
