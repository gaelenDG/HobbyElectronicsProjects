// ======== Library initialization ========
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_PN532.h>
#include <Wire.h>
#include <SPI.h>
#include <cstdint>
#include "DFRobotDFPlayerMini.h"
#include "config.h"

// using namespace std;

// ======== GPIO & SPI & UART Declarations ========

uint8_t StatusLight_R_Pin = 0;
uint8_t StatusLight_G_Pin = 1;
uint8_t StatusLight_B_Pin = 2;

const int Button1_pin = 22; // GPIO pin for capacitive touch button data line
const int Button2_pin = 23; // GPIO pin for capacitive touch button data line

uint8_t Switch_pin = 5; // Deep sleep switch

const int SPI_MISO_PIN = 20; // Master In Slave Out
const int SPI_MOSI_PIN = 18;// Master out slave in
const int SPI_SCK_PIN = 19; // Serial Clock
const int SPI_CS_PIN = 21;  // Chip Select

const int UART_TX_pin = 16; // DFPlayer Mini TX -> RX
const int UART_RX_pin = 17; // DFPlayer Mini RX -> TX

// ======== WiFi & MQTT client info ========

WiFiClient espClient;
PubSubClient mqttClient(espClient);


// ======== MQTT Topics ========

String logMsg;

// ======== Peripheral state checks ========

bool dfplayerOK = false;
bool nfcOK = false;
unsigned long lastPeripheralCheck = 0;
const unsigned long CHECK_INTERVAL = 10000; // every 10 seconds



// ======== Voltage Reader & Battery control ========
uint8_t VoltageReader_Pin = 4;
unsigned long lastBattCheck = 0;
uint64_t Batt_Check_Interval = 60000UL;
float LOW_BAT_THRESHOLD = 3.4; // A limit point to trigger deep sleep if the battery is too low
uint64_t LOW_BAT_SLEEP_INTERVAL = 1; // Minutes between battery checks
int WAKE_BAT_THRESHOLD = 3.6; // threshold for sufficient battery voltage to wake back up
float NO_BAT_THRESHOLD = 2.0;

// ======== Status light, volume, & Buttons ========

uint64_t LONG_PRESS_TIME = 800;
uint64_t REPEAT_INTERVAL = 500;
uint64_t DEBOUNCE_TIME = 50;

int volume = 15;
int MAX_VOLUME = 30;
int MIN_VOLUME = 0;


// ======== RFID reader ========

Adafruit_PN532 nfc(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_CS_PIN);

unsigned long lastNfcCheck = 0;
const unsigned long nfcInterval = 3000;


// ======== DF Player Mini ========

HardwareSerial MP3Serial(1);

DFRobotDFPlayerMini player;

bool hasPlayedForCurrentTag = false; // check to trigger playback only once per chip reading
unsigned long lastTagSeen = 0;
bool tagPresent = false;
uint8_t currentUID[7] = {0};
uint8_t currentUIDLength = 0;

const unsigned long TAG_TIMEOUT = 1200UL; // ms to wait for lost tag
const unsigned long POST_READ_COOLDOWN = 600UL; // ms to wait after a successful read


