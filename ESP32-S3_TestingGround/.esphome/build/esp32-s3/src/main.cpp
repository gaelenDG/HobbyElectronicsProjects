// Auto generated code by esphome
// ========== AUTO GENERATED INCLUDE BLOCK BEGIN ===========
#include "esphome.h"
using namespace esphome;
using std::isnan;
using std::min;
using std::max;
using namespace sensor;
logger::Logger *logger_logger_id;
web_server_base::WebServerBase *web_server_base_webserverbase_id;
captive_portal::CaptivePortal *captive_portal_captiveportal_id;
wifi::WiFiComponent *wifi_wificomponent_id;
mdns::MDNSComponent *mdns_mdnscomponent_id;
esphome::ESPHomeOTAComponent *esphome_esphomeotacomponent_id;
safe_mode::SafeModeComponent *safe_mode_safemodecomponent_id;
api::APIServer *api_apiserver_id;
using namespace api;
using namespace i2c;
i2c::ArduinoI2CBus *bus_a;
preferences::IntervalSyncer *preferences_intervalsyncer_id;
aht10::AHT10Component *aht10_aht10component_id;
sensor::Sensor *sensor_sensor_id;
sensor::Sensor *sensor_sensor_id_2;
bmp280_i2c::BMP280I2CComponent *bmp280_i2c_bmp280i2ccomponent_id;
sensor::Sensor *sensor_sensor_id_3;
sensor::Sensor *sensor_sensor_id_4;
#define yield() esphome::yield()
#define millis() esphome::millis()
#define micros() esphome::micros()
#define delay(x) esphome::delay(x)
#define delayMicroseconds(x) esphome::delayMicroseconds(x)
// ========== AUTO GENERATED INCLUDE BLOCK END ==========="

void setup() {
  // ========== AUTO GENERATED CODE BEGIN ===========
  App.reserve_sensor(4);
  // async_tcp:
  //   {}
  // esphome:
  //   name: esp32-s3
  //   min_version: 2025.6.0
  //   build_path: build/esp32-s3
  //   friendly_name: ''
  //   area: ''
  //   platformio_options: {}
  //   includes: []
  //   libraries: []
  //   name_add_mac_suffix: false
  //   debug_scheduler: false
  App.pre_setup("esp32-s3", "", "", "", __DATE__ ", " __TIME__, false);
  App.reserve_components(12);
  // sensor:
  // logger:
  //   id: logger_logger_id
  //   baud_rate: 115200
  //   tx_buffer_size: 512
  //   deassert_rts_dtr: false
  //   task_log_buffer_size: 768
  //   hardware_uart: USB_CDC
  //   level: DEBUG
  //   logs: {}
  logger_logger_id = new logger::Logger(115200, 512);
  logger_logger_id->create_pthread_key();
  logger_logger_id->init_log_buffer(768);
  logger_logger_id->set_log_level(ESPHOME_LOG_LEVEL_DEBUG);
  logger_logger_id->set_uart_selection(logger::UART_SELECTION_USB_CDC);
  logger_logger_id->pre_setup();
  logger_logger_id->set_component_source("logger");
  App.register_component(logger_logger_id);
  // web_server_base:
  //   id: web_server_base_webserverbase_id
  web_server_base_webserverbase_id = new web_server_base::WebServerBase();
  web_server_base_webserverbase_id->set_component_source("web_server_base");
  App.register_component(web_server_base_webserverbase_id);
  // captive_portal:
  //   id: captive_portal_captiveportal_id
  //   web_server_base_id: web_server_base_webserverbase_id
  captive_portal_captiveportal_id = new captive_portal::CaptivePortal(web_server_base_webserverbase_id);
  captive_portal_captiveportal_id->set_component_source("captive_portal");
  App.register_component(captive_portal_captiveportal_id);
  // wifi:
  //   ap:
  //     ssid: Esp32-S3 Fallback Hotspot
  //     password: I8kwRQsVDp7u
  //     id: wifi_wifiap_id
  //     ap_timeout: 1min
  //   id: wifi_wificomponent_id
  //   domain: .local
  //   reboot_timeout: 15min
  //   power_save_mode: LIGHT
  //   fast_connect: false
  //   passive_scan: false
  //   enable_on_boot: true
  //   networks:
  //     - ssid: WendellsWorld
  //       password: partytime
  //       id: wifi_wifiap_id_2
  //       priority: 0.0
  //   use_address: esp32-s3.local
  wifi_wificomponent_id = new wifi::WiFiComponent();
  wifi_wificomponent_id->set_use_address("esp32-s3.local");
  {
  wifi::WiFiAP wifi_wifiap_id_2 = wifi::WiFiAP();
  wifi_wifiap_id_2.set_ssid("WendellsWorld");
  wifi_wifiap_id_2.set_password("partytime");
  wifi_wifiap_id_2.set_priority(0.0f);
  wifi_wificomponent_id->add_sta(wifi_wifiap_id_2);
  }
  {
  wifi::WiFiAP wifi_wifiap_id = wifi::WiFiAP();
  wifi_wifiap_id.set_ssid("Esp32-S3 Fallback Hotspot");
  wifi_wifiap_id.set_password("I8kwRQsVDp7u");
  wifi_wificomponent_id->set_ap(wifi_wifiap_id);
  }
  wifi_wificomponent_id->set_ap_timeout(60000);
  wifi_wificomponent_id->set_reboot_timeout(900000);
  wifi_wificomponent_id->set_power_save_mode(wifi::WIFI_POWER_SAVE_LIGHT);
  wifi_wificomponent_id->set_fast_connect(false);
  wifi_wificomponent_id->set_passive_scan(false);
  wifi_wificomponent_id->set_enable_on_boot(true);
  wifi_wificomponent_id->set_component_source("wifi");
  App.register_component(wifi_wificomponent_id);
  // mdns:
  //   id: mdns_mdnscomponent_id
  //   disabled: false
  //   services: []
  mdns_mdnscomponent_id = new mdns::MDNSComponent();
  mdns_mdnscomponent_id->set_component_source("mdns");
  App.register_component(mdns_mdnscomponent_id);
  // ota:
  // ota.esphome:
  //   platform: esphome
  //   password: papanoel10
  //   id: esphome_esphomeotacomponent_id
  //   version: 2
  //   port: 3232
  esphome_esphomeotacomponent_id = new esphome::ESPHomeOTAComponent();
  esphome_esphomeotacomponent_id->set_port(3232);
  esphome_esphomeotacomponent_id->set_auth_password("papanoel10");
  esphome_esphomeotacomponent_id->set_component_source("esphome.ota");
  App.register_component(esphome_esphomeotacomponent_id);
  // safe_mode:
  //   id: safe_mode_safemodecomponent_id
  //   boot_is_good_after: 1min
  //   disabled: false
  //   num_attempts: 10
  //   reboot_timeout: 5min
  safe_mode_safemodecomponent_id = new safe_mode::SafeModeComponent();
  safe_mode_safemodecomponent_id->set_component_source("safe_mode");
  App.register_component(safe_mode_safemodecomponent_id);
  if (safe_mode_safemodecomponent_id->should_enter_safe_mode(10, 300000, 60000)) return;
  // api:
  //   password: papanoel10
  //   id: api_apiserver_id
  //   port: 6053
  //   reboot_timeout: 15min
  //   batch_delay: 100ms
  api_apiserver_id = new api::APIServer();
  api_apiserver_id->set_component_source("api");
  App.register_component(api_apiserver_id);
  api_apiserver_id->set_port(6053);
  api_apiserver_id->set_password("papanoel10");
  api_apiserver_id->set_reboot_timeout(900000);
  api_apiserver_id->set_batch_delay(100);
  // i2c:
  //   sda: 1
  //   scl: 2
  //   scan: true
  //   id: bus_a
  //   frequency: 50000.0
  bus_a = new i2c::ArduinoI2CBus();
  bus_a->set_component_source("i2c");
  App.register_component(bus_a);
  bus_a->set_sda_pin(1);
  bus_a->set_scl_pin(2);
  bus_a->set_frequency(50000);
  bus_a->set_scan(true);
  // esp32:
  //   board: esp32-s3-devkitc-1
  //   framework:
  //     version: 2.0.5
  //     advanced:
  //       ignore_efuse_custom_mac: false
  //     source: ~3.20005.0
  //     platform_version: platformio/espressif32@5.4.0
  //     type: arduino
  //   flash_size: 4MB
  //   variant: ESP32S3
  //   cpu_frequency: 160MHZ
  setCpuFrequencyMhz(160);
  // preferences:
  //   id: preferences_intervalsyncer_id
  //   flash_write_interval: 60s
  preferences_intervalsyncer_id = new preferences::IntervalSyncer();
  preferences_intervalsyncer_id->set_write_interval(60000);
  preferences_intervalsyncer_id->set_component_source("preferences");
  App.register_component(preferences_intervalsyncer_id);
  // sensor.aht10:
  //   platform: aht10
  //   temperature:
  //     name: AHT20 Temperature
  //     disabled_by_default: false
  //     id: sensor_sensor_id
  //     force_update: false
  //     unit_of_measurement: °C
  //     accuracy_decimals: 2
  //     device_class: temperature
  //     state_class: measurement
  //   humidity:
  //     name: AHT20 Humidity
  //     disabled_by_default: false
  //     id: sensor_sensor_id_2
  //     force_update: false
  //     unit_of_measurement: '%'
  //     accuracy_decimals: 2
  //     device_class: humidity
  //     state_class: measurement
  //   update_interval: 60s
  //   id: aht10_aht10component_id
  //   variant: AHT10
  //   i2c_id: bus_a
  //   address: 0x38
  aht10_aht10component_id = new aht10::AHT10Component();
  aht10_aht10component_id->set_update_interval(60000);
  aht10_aht10component_id->set_component_source("aht10.sensor");
  App.register_component(aht10_aht10component_id);
  aht10_aht10component_id->set_i2c_bus(bus_a);
  aht10_aht10component_id->set_i2c_address(0x38);
  aht10_aht10component_id->set_variant(aht10::AHT10);
  sensor_sensor_id = new sensor::Sensor();
  App.register_sensor(sensor_sensor_id);
  sensor_sensor_id->set_name("AHT20 Temperature");
  sensor_sensor_id->set_object_id("aht20_temperature");
  sensor_sensor_id->set_disabled_by_default(false);
  sensor_sensor_id->set_device_class("temperature");
  sensor_sensor_id->set_state_class(sensor::STATE_CLASS_MEASUREMENT);
  sensor_sensor_id->set_unit_of_measurement("\302\260C");
  sensor_sensor_id->set_accuracy_decimals(2);
  sensor_sensor_id->set_force_update(false);
  aht10_aht10component_id->set_temperature_sensor(sensor_sensor_id);
  sensor_sensor_id_2 = new sensor::Sensor();
  App.register_sensor(sensor_sensor_id_2);
  sensor_sensor_id_2->set_name("AHT20 Humidity");
  sensor_sensor_id_2->set_object_id("aht20_humidity");
  sensor_sensor_id_2->set_disabled_by_default(false);
  sensor_sensor_id_2->set_device_class("humidity");
  sensor_sensor_id_2->set_state_class(sensor::STATE_CLASS_MEASUREMENT);
  sensor_sensor_id_2->set_unit_of_measurement("%");
  sensor_sensor_id_2->set_accuracy_decimals(2);
  sensor_sensor_id_2->set_force_update(false);
  aht10_aht10component_id->set_humidity_sensor(sensor_sensor_id_2);
  // sensor.bmp280_i2c:
  //   platform: bmp280_i2c
  //   temperature:
  //     name: BMP280 Temperature
  //     disabled_by_default: false
  //     id: sensor_sensor_id_3
  //     force_update: false
  //     unit_of_measurement: °C
  //     accuracy_decimals: 1
  //     device_class: temperature
  //     state_class: measurement
  //     oversampling: 16X
  //   pressure:
  //     name: BMP280 Pressure
  //     disabled_by_default: false
  //     id: sensor_sensor_id_4
  //     force_update: false
  //     unit_of_measurement: hPa
  //     accuracy_decimals: 1
  //     device_class: pressure
  //     state_class: measurement
  //     oversampling: 16X
  //   address: 0x76
  //   update_interval: 60s
  //   iir_filter: 'OFF'
  //   i2c_id: bus_a
  //   id: bmp280_i2c_bmp280i2ccomponent_id
  bmp280_i2c_bmp280i2ccomponent_id = new bmp280_i2c::BMP280I2CComponent();
  bmp280_i2c_bmp280i2ccomponent_id->set_update_interval(60000);
  bmp280_i2c_bmp280i2ccomponent_id->set_component_source("bmp280_base");
  App.register_component(bmp280_i2c_bmp280i2ccomponent_id);
  sensor_sensor_id_3 = new sensor::Sensor();
  App.register_sensor(sensor_sensor_id_3);
  sensor_sensor_id_3->set_name("BMP280 Temperature");
  sensor_sensor_id_3->set_object_id("bmp280_temperature");
  sensor_sensor_id_3->set_disabled_by_default(false);
  sensor_sensor_id_3->set_device_class("temperature");
  sensor_sensor_id_3->set_state_class(sensor::STATE_CLASS_MEASUREMENT);
  sensor_sensor_id_3->set_unit_of_measurement("\302\260C");
  sensor_sensor_id_3->set_accuracy_decimals(1);
  sensor_sensor_id_3->set_force_update(false);
  bmp280_i2c_bmp280i2ccomponent_id->set_temperature_sensor(sensor_sensor_id_3);
  bmp280_i2c_bmp280i2ccomponent_id->set_temperature_oversampling(bmp280_base::BMP280_OVERSAMPLING_16X);
  sensor_sensor_id_4 = new sensor::Sensor();
  App.register_sensor(sensor_sensor_id_4);
  sensor_sensor_id_4->set_name("BMP280 Pressure");
  sensor_sensor_id_4->set_object_id("bmp280_pressure");
  sensor_sensor_id_4->set_disabled_by_default(false);
  sensor_sensor_id_4->set_device_class("pressure");
  sensor_sensor_id_4->set_state_class(sensor::STATE_CLASS_MEASUREMENT);
  sensor_sensor_id_4->set_unit_of_measurement("hPa");
  sensor_sensor_id_4->set_accuracy_decimals(1);
  sensor_sensor_id_4->set_force_update(false);
  bmp280_i2c_bmp280i2ccomponent_id->set_pressure_sensor(sensor_sensor_id_4);
  bmp280_i2c_bmp280i2ccomponent_id->set_pressure_oversampling(bmp280_base::BMP280_OVERSAMPLING_16X);
  bmp280_i2c_bmp280i2ccomponent_id->set_iir_filter(bmp280_base::BMP280_IIR_FILTER_OFF);
  bmp280_i2c_bmp280i2ccomponent_id->set_i2c_bus(bus_a);
  bmp280_i2c_bmp280i2ccomponent_id->set_i2c_address(0x76);
  // socket:
  //   implementation: bsd_sockets
  // md5:
  // network:
  //   enable_ipv6: false
  //   min_ipv6_addr_count: 0
  // =========== AUTO GENERATED CODE END ============
  App.setup();
}

void loop() {
  App.loop();
}
