// helpers.h - function prototypes for helpers.cpp

#ifndef HELPERS_H
#define HELPERS_H

// ======== Library initialization ========
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_PN532.h>

// ======== Function prototypes ======== //

// All defined in helpers.cpp

int readRFID();
int readVolumeKnob();
String readNdefTextFromTag();
bool readTagPage(uint8_t page, uint8_t *buf);
float readBatVoltage();
void checkBatteryAndSleepIfLow();
bool checkDFPlayerConnection();
void checkPeripherals();
void setStatusLight(uint8_t RedVal, uint8_t GreenVal, uint8_t BlueVal);
void connectToWiFi();
void connectToMQTT();
void mqttLog(const String& message);
void mqttCallback(char* topic, byte* message, unsigned int length);


#pragma once

enum ButtonAction {
  BUTTON_NONE,
  BUTTON_SHORT,
  BUTTON_LONG,
  BUTTON_REPEAT
};

ButtonAction checkButton(uint8_t pin);
void handleButtonAction();

#endif // HELPERS_H