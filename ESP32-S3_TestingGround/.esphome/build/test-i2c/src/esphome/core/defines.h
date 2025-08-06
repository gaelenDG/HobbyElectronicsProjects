#pragma once
#include "esphome/core/macros.h"
#define ESPHOME_BOARD "esp32-s3-devkitc-1"
#define ESPHOME_VARIANT "ESP32-S3"
#define USE_CAPTIVE_PORTAL
#define USE_ESPHOME_TASK_LOG_BUFFER
#define USE_ESP_IDF_VERSION_CODE VERSION_CODE(5, 3, 2)
#define USE_LOGGER
#define USE_LOGGER_USB_CDC
#define USE_LOGGER_USB_SERIAL_JTAG
#define USE_MD5
#define USE_MDNS
#define USE_NETWORK
#define USE_NETWORK_IPV6 false
#define USE_OTA
#define USE_OTA_PASSWORD
#define USE_OTA_VERSION 2
#define USE_SOCKET_IMPL_BSD_SOCKETS
#define USE_SOCKET_SELECT_SUPPORT
#define USE_WIFI
