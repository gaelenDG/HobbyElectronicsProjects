// Auto generated code by esphome
// ========== AUTO GENERATED INCLUDE BLOCK BEGIN ===========
#include "esphome.h"
using namespace esphome;
using std::isnan;
using std::min;
using std::max;
using namespace light;
using namespace time;
logger::Logger *logger_logger_id;
web_server_base::WebServerBase *web_server_base_webserverbase_id;
captive_portal::CaptivePortal *captive_portal_captiveportal_id;
wifi::WiFiComponent *wifi_wificomponent_id;
mdns::MDNSComponent *mdns_mdnscomponent_id;
esphome::ESPHomeOTAComponent *esphome_esphomeotacomponent_id;
safe_mode::SafeModeComponent *safe_mode_safemodecomponent_id;
api::APIServer *api_apiserver_id;
using namespace api;
preferences::IntervalSyncer *preferences_intervalsyncer_id;
neopixelbus::NeoPixelRGBWLightOutput<NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedSk6812, NeoEsp32RmtChannel1>> *neopixelbus_neopixelbuslightoutputbase_id;
light::AddressableLightState *weather_strip;
light::AddressableLambdaLightEffect *light_addressablelambdalighteffect_id;
light::AddressableLambdaLightEffect *light_addressablelambdalighteffect_id_2;
light::RandomLightEffect *light_randomlighteffect_id;
homeassistant::HomeassistantTime *ha_time;
time::CronTrigger *time_crontrigger_id;
Automation<> *automation_id;
light::LightControlAction<> *light_lightcontrolaction_id;
sun::Sun *sun_sun_id;
sun::SunTrigger *sun_suntrigger_id;
Automation<> *automation_id_2;
light::LightControlAction<> *light_lightcontrolaction_id_2;
sun::SunTrigger *sun_suntrigger_id_2;
Automation<> *automation_id_3;
light::LightControlAction<> *light_lightcontrolaction_id_3;
#define yield() esphome::yield()
#define millis() esphome::millis()
#define micros() esphome::micros()
#define delay(x) esphome::delay(x)
#define delayMicroseconds(x) esphome::delayMicroseconds(x)
// ========== AUTO GENERATED INCLUDE BLOCK END ==========="

void setup() {
  // ========== AUTO GENERATED CODE BEGIN ===========
  App.reserve_light(1);
  // async_tcp:
  //   {}
  // esphome:
  //   name: esp32-c3
  //   friendly_name: esp32-c3
  //   min_version: 2025.6.0
  //   build_path: build/esp32-c3
  //   area: ''
  //   platformio_options: {}
  //   includes: []
  //   libraries: []
  //   name_add_mac_suffix: false
  //   debug_scheduler: false
  App.pre_setup("esp32-c3", "esp32-c3", "", "", __DATE__ ", " __TIME__, false);
  App.reserve_components(15);
  // light:
  // time:
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
  //     ssid: Esp32-C3 Fallback Hotspot
  //     password: Z8dTshYv61zn
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
  //     - ssid: !secret 'wifi_ssid'
  //       password: !secret 'wifi_password'
  //       id: wifi_wifiap_id_2
  //       priority: 0.0
  //   use_address: esp32-c3.local
  wifi_wificomponent_id = new wifi::WiFiComponent();
  wifi_wificomponent_id->set_use_address("esp32-c3.local");
  {
  wifi::WiFiAP wifi_wifiap_id_2 = wifi::WiFiAP();
  wifi_wifiap_id_2.set_ssid("WendellsWorld");
  wifi_wifiap_id_2.set_password("partytime");
  wifi_wifiap_id_2.set_priority(0.0f);
  wifi_wificomponent_id->add_sta(wifi_wifiap_id_2);
  }
  {
  wifi::WiFiAP wifi_wifiap_id = wifi::WiFiAP();
  wifi_wifiap_id.set_ssid("Esp32-C3 Fallback Hotspot");
  wifi_wifiap_id.set_password("Z8dTshYv61zn");
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
  //   password: 0614ccd5c6b85ff2eefaba06b0456e68
  //   id: esphome_esphomeotacomponent_id
  //   version: 2
  //   port: 3232
  esphome_esphomeotacomponent_id = new esphome::ESPHomeOTAComponent();
  esphome_esphomeotacomponent_id->set_port(3232);
  esphome_esphomeotacomponent_id->set_auth_password("0614ccd5c6b85ff2eefaba06b0456e68");
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
  //   encryption:
  //     key: GB3Wpi0KuWDjoW8UCjc2pMujljfTgDdR86TkAFbrVlc=
  //   id: api_apiserver_id
  //   port: 6053
  //   password: ''
  //   reboot_timeout: 15min
  //   batch_delay: 100ms
  api_apiserver_id = new api::APIServer();
  api_apiserver_id->set_component_source("api");
  App.register_component(api_apiserver_id);
  api_apiserver_id->set_port(6053);
  api_apiserver_id->set_password("");
  api_apiserver_id->set_reboot_timeout(900000);
  api_apiserver_id->set_batch_delay(100);
  api_apiserver_id->set_noise_psk({24, 29, 214, 166, 45, 10, 185, 96, 227, 161, 111, 20, 10, 55, 54, 164, 203, 163, 150, 55, 211, 128, 55, 81, 243, 164, 228, 0, 86, 235, 86, 87});
  // esp32:
  //   board: esp32-c3-devkitm-1
  //   framework:
  //     version: 2.0.5
  //     advanced:
  //       ignore_efuse_custom_mac: false
  //     source: ~3.20005.0
  //     platform_version: platformio/espressif32@5.4.0
  //     type: arduino
  //   flash_size: 4MB
  //   variant: ESP32C3
  //   cpu_frequency: 160MHZ
  setCpuFrequencyMhz(160);
  // preferences:
  //   id: preferences_intervalsyncer_id
  //   flash_write_interval: 60s
  preferences_intervalsyncer_id = new preferences::IntervalSyncer();
  preferences_intervalsyncer_id->set_write_interval(60000);
  preferences_intervalsyncer_id->set_component_source("preferences");
  App.register_component(preferences_intervalsyncer_id);
  // light.neopixelbus:
  //   platform: neopixelbus
  //   type: GRBW
  //   variant: sk6812
  //   pin: 2
  //   num_leds: 10
  //   name: ESP32-C3 RGB LED
  //   id: weather_strip
  //   effects:
  //     - addressable_lambda:
  //         name: Sunrise
  //         update_interval: 1s
  //         lambda: !lambda |-
  //           static bool initial = true;
  //           static float elapsed = 0;
  //           if (initial) {
  //             elapsed = 0;
  //             initial = false;
  //           } else {
  //             elapsed += 1.0;   1 second per update
  //           }
  //   
  //           const float duration = 60.0;   30 min total
  //           float pos = elapsed / duration;
  //           if (pos > 1.0) pos = 1.0;
  //   
  //           uint8_t r, g, b, w;
  //   
  //           if (pos < 0.3) {
  //              Stage 1: black → pink
  //             float t = pos / 0.3;
  //             r = uint8_t(255 * t);
  //             g = uint8_t(100 * t);
  //             b = uint8_t(80 * t);
  //             w = 0;
  //           } else if (pos < 0.7) {
  //              Stage 2: pink → orange
  //             float t = (pos - 0.3) / 0.4;
  //             r = 255;
  //             g = uint8_t(100 + (t * 100));   to 200
  //             b = uint8_t(t * 128);
  //             w = 0;
  //           } else {
  //              Stage 3: orange → warm white
  //             float t = (pos - 0.7) / 0.3;
  //             r = 255;
  //             g = uint8_t(100 * (1 - t) + 200 * t);
  //             b = uint8_t(50 * (1 - t));
  //             w = uint8_t(180 * t);
  //           }
  //   
  //           it.all() = Color(r, g, b);
  //       type_id: light_addressablelambdalighteffect_id
  //     - addressable_lambda:
  //         name: Sunset
  //         update_interval: 1s
  //         lambda: !lambda "static bool initial = true;\nstatic float elapsed = 0;\nif
  //           \ (initial) {\n  elapsed = 0;\n  initial = false;\n} else {\n  elapsed +=
  //           \ 1.0;   1 second per update\n}\n\nconst float duration = 60.0;   30 min
  //           \ total\nfloat pos = elapsed / duration;\nif (pos > 1.0) pos = 1.0;\npos =
  //           \ 1.0 - pos;\n\n RGB values\nuint8_t r=0, g=0, b=0, w=0; \nif (pos > 0.7)
  //           \ {\n   Stage 1: Warm white → soft orange\n  float t = (pos - 0.7) / 0.3;\n
  //           \  r = 255;\n  g = uint8_t(200 * t + 100 * (1 - t));   down from yellow
  //           \ to orange\n  b = uint8_t(50 * t);                    small blue for warmth\n
  //           \  w = uint8_t(180 * t);\n} else if (pos > 0.3) {\n   Stage 2: Orange →
  //           \ light pink\n  float t = (pos - 0.3) / 0.4;\n  r = 255;\n  g = uint8_t(100
  //           \ + (t * 100));          100 → 200\n  b = uint8_t(t * 128);\n  w = 0;\n
  //           } else {\n   Stage 3: Pink → off\n  float t = pos / 0.3;\n  r = uint8_t(255
  //           \ * t);\n  g = uint8_t(200 * t);\n  b = uint8_t(128 * t);\n  w = 0;\n}\n\n
  //           \nit.all() = Color(r, g, b);"
  //       type_id: light_addressablelambdalighteffect_id_2
  //     - random:
  //         name: Weather
  //         transition_length: 5s
  //         update_interval: 1min
  //       type_id: light_randomlighteffect_id
  //   disabled_by_default: false
  //   restore_mode: ALWAYS_OFF
  //   gamma_correct: 2.8
  //   default_transition_length: 1s
  //   flash_transition_length: 0s
  //   output_id: neopixelbus_neopixelbuslightoutputbase_id
  //   invert: false
  //   method:
  //     channel: 1
  //     type: esp32_rmt
  neopixelbus_neopixelbuslightoutputbase_id = new neopixelbus::NeoPixelRGBWLightOutput<NeoEsp32RmtMethodBase<NeoEsp32RmtSpeedSk6812, NeoEsp32RmtChannel1>>();
  weather_strip = new light::AddressableLightState(neopixelbus_neopixelbuslightoutputbase_id);
  App.register_light(weather_strip);
  weather_strip->set_component_source("light");
  App.register_component(weather_strip);
  weather_strip->set_name("ESP32-C3 RGB LED");
  weather_strip->set_object_id("esp32-c3_rgb_led");
  weather_strip->set_disabled_by_default(false);
  weather_strip->set_restore_mode(light::LIGHT_ALWAYS_OFF);
  weather_strip->set_default_transition_length(1000);
  weather_strip->set_flash_transition_length(0);
  weather_strip->set_gamma_correct(2.8f);
  light_addressablelambdalighteffect_id = new light::AddressableLambdaLightEffect("Sunrise", [=](light::AddressableLight & it, Color current_color, bool initial_run) -> void {
      #line 48 "./esp32-c3.yaml"
      static bool initial = true;
      static float elapsed = 0;
      if (initial) {
        elapsed = 0;
        initial = false;
      } else {
        elapsed += 1.0;   
      }
      
      const float duration = 60.0;   
      float pos = elapsed / duration;
      if (pos > 1.0) pos = 1.0;
      
      uint8_t r, g, b, w;
      
      if (pos < 0.3) {
         
        float t = pos / 0.3;
        r = uint8_t(255 * t);
        g = uint8_t(100 * t);
        b = uint8_t(80 * t);
        w = 0;
      } else if (pos < 0.7) {
         
        float t = (pos - 0.3) / 0.4;
        r = 255;
        g = uint8_t(100 + (t * 100));   
        b = uint8_t(t * 128);
        w = 0;
      } else {
         
        float t = (pos - 0.7) / 0.3;
        r = 255;
        g = uint8_t(100 * (1 - t) + 200 * t);
        b = uint8_t(50 * (1 - t));
        w = uint8_t(180 * t);
      }
      
      it.all() = Color(r, g, b);
  }, 1000);
  light_addressablelambdalighteffect_id_2 = new light::AddressableLambdaLightEffect("Sunset", [=](light::AddressableLight & it, Color current_color, bool initial_run) -> void {
      #line 93 "./esp32-c3.yaml"
      static bool initial = true;
      static float elapsed = 0;
      if (initial) {
        elapsed = 0;
        initial = false;
      } else {
        elapsed += 1.0;   
      }
      
      const float duration = 60.0;   
      float pos = elapsed / duration;
      if (pos > 1.0) pos = 1.0;
      pos = 1.0 - pos;
      
       
      uint8_t r=0, g=0, b=0, w=0; 
      if (pos > 0.7) {
         
        float t = (pos - 0.7) / 0.3;
        r = 255;
        g = uint8_t(200 * t + 100 * (1 - t));   
        b = uint8_t(50 * t);                    
        w = uint8_t(180 * t);
      } else if (pos > 0.3) {
         
        float t = (pos - 0.3) / 0.4;
        r = 255;
        g = uint8_t(100 + (t * 100));          
        b = uint8_t(t * 128);
        w = 0;
      } else {
         
        float t = pos / 0.3;
        r = uint8_t(255 * t);
        g = uint8_t(200 * t);
        b = uint8_t(128 * t);
        w = 0;
      }
      
      
      it.all() = Color(r, g, b);
  }, 1000);
  light_randomlighteffect_id = new light::RandomLightEffect("Weather");
  light_randomlighteffect_id->set_transition_length(5000);
  light_randomlighteffect_id->set_update_interval(60000);
  weather_strip->add_effects({light_addressablelambdalighteffect_id, light_addressablelambdalighteffect_id_2, light_randomlighteffect_id});
  neopixelbus_neopixelbuslightoutputbase_id->set_component_source("neopixelbus.light");
  App.register_component(neopixelbus_neopixelbuslightoutputbase_id);
  neopixelbus_neopixelbuslightoutputbase_id->add_leds(10, 2);
  neopixelbus_neopixelbuslightoutputbase_id->set_pixel_order(neopixelbus::ESPNeoPixelOrder::GRBW);
  // time.homeassistant:
  //   platform: homeassistant
  //   id: ha_time
  //   on_time:
  //     - seconds:
  //         - 0
  //       minutes:
  //         - 0
  //       hours:
  //         - 8
  //       then:
  //         - light.turn_on:
  //             id: weather_strip
  //             effect: Weather
  //             state: true
  //           type_id: light_lightcontrolaction_id
  //       automation_id: automation_id
  //       trigger_id: time_crontrigger_id
  //   timezone: PST8PDT,M3.2.0,M11.1.0
  //   update_interval: 15min
  ha_time = new homeassistant::HomeassistantTime();
  ha_time->set_timezone("PST8PDT,M3.2.0,M11.1.0");
  time_crontrigger_id = new time::CronTrigger(ha_time);
  time_crontrigger_id->add_seconds({0});
  time_crontrigger_id->add_minutes({0});
  time_crontrigger_id->add_hours({8});
  time_crontrigger_id->add_days_of_month({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31});
  time_crontrigger_id->add_months({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  time_crontrigger_id->add_days_of_week({1, 2, 3, 4, 5, 6, 7});
  time_crontrigger_id->set_component_source("time");
  App.register_component(time_crontrigger_id);
  automation_id = new Automation<>(time_crontrigger_id);
  light_lightcontrolaction_id = new light::LightControlAction<>(weather_strip);
  light_lightcontrolaction_id->set_state(true);
  light_lightcontrolaction_id->set_effect("Weather");
  automation_id->add_actions({light_lightcontrolaction_id});
  ha_time->set_update_interval(900000);
  ha_time->set_component_source("homeassistant.time");
  App.register_component(ha_time);
  // sun:
  //   latitude: 40.123456
  //   longitude: -122.345678
  //   on_sunrise:
  //     - then:
  //         - light.turn_on:
  //             id: weather_strip
  //             effect: Sunrise
  //             state: true
  //           type_id: light_lightcontrolaction_id_2
  //       automation_id: automation_id_2
  //       trigger_id: sun_suntrigger_id
  //       elevation: -0.83333
  //   on_sunset:
  //     - then:
  //         - light.turn_on:
  //             id: weather_strip
  //             effect: Sunset
  //             state: true
  //           type_id: light_lightcontrolaction_id_3
  //       automation_id: automation_id_3
  //       trigger_id: sun_suntrigger_id_2
  //       elevation: -0.83333
  //   id: sun_sun_id
  //   time_id: ha_time
  sun_sun_id = new sun::Sun();
  sun_sun_id->set_time(ha_time);
  sun_sun_id->set_latitude(40.123456f);
  sun_sun_id->set_longitude(-122.345678f);
  sun_suntrigger_id = new sun::SunTrigger();
  sun_suntrigger_id->set_component_source("sun");
  App.register_component(sun_suntrigger_id);
  sun_suntrigger_id->set_parent(sun_sun_id);
  sun_suntrigger_id->set_sunrise(true);
  sun_suntrigger_id->set_elevation(-0.83333f);
  automation_id_2 = new Automation<>(sun_suntrigger_id);
  light_lightcontrolaction_id_2 = new light::LightControlAction<>(weather_strip);
  light_lightcontrolaction_id_2->set_state(true);
  light_lightcontrolaction_id_2->set_effect("Sunrise");
  automation_id_2->add_actions({light_lightcontrolaction_id_2});
  sun_suntrigger_id_2 = new sun::SunTrigger();
  sun_suntrigger_id_2->set_component_source("sun");
  App.register_component(sun_suntrigger_id_2);
  sun_suntrigger_id_2->set_parent(sun_sun_id);
  sun_suntrigger_id_2->set_sunrise(false);
  sun_suntrigger_id_2->set_elevation(-0.83333f);
  automation_id_3 = new Automation<>(sun_suntrigger_id_2);
  light_lightcontrolaction_id_3 = new light::LightControlAction<>(weather_strip);
  light_lightcontrolaction_id_3->set_state(true);
  light_lightcontrolaction_id_3->set_effect("Sunset");
  automation_id_3->add_actions({light_lightcontrolaction_id_3});
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
