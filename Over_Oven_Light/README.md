# ESP32C3 NeoPixel MQTT Controller

This project allows you to control an array of NeoPixel RGB LEDs using an ESP32C3 microcontroller. It connects to Home Assistant via the MQTT protocol, enabling you to adjust the brightness of the LEDs through a dimmer slider in the Home Assistant interface.

## Project Structure

```
esp32c3-neopixel-mqtt
├── src
│   ├── main.cpp          # Main script for controlling NeoPixels and MQTT
├── lib
│   ├── Adafruit_NeoPixel # Library for controlling NeoPixel LEDs
│   └── PubSubClient      # Library for MQTT communication
├── platformio.ini       # PlatformIO configuration file
└── README.md             # Project documentation
```

## Requirements

- ESP32C3 microcontroller
- NeoPixel RGB LED strip
- Home Assistant setup with MQTT broker

## Setup Instructions

1. **Install PlatformIO**: Ensure you have PlatformIO installed in your development environment.

2. **Clone the Repository**: Clone this repository to your local machine.

3. **Install Libraries**: The required libraries (Adafruit NeoPixel and PubSubClient) are included in the `lib` directory. Ensure they are correctly referenced in your `platformio.ini` file.

4. **Configure Wi-Fi and MQTT**: 
   - Open `src/main.cpp` and set your Wi-Fi credentials and MQTT broker details.
   - Update the MQTT topic to match your Home Assistant configuration.

5. **Upload the Code**: Connect your ESP32C3 to your computer and upload the code using PlatformIO.

## Usage

- Once the ESP32C3 is connected to your Wi-Fi and MQTT broker, you can control the NeoPixel LEDs from Home Assistant.
- Use the dimmer slider in Home Assistant to adjust the brightness of the LEDs.
- The current brightness state will be published to the specified MQTT topic.

## Troubleshooting

- Ensure your ESP32C3 is properly connected to the Wi-Fi network.
- Verify that the MQTT broker is running and accessible.
- Check the Home Assistant logs for any errors related to MQTT communication.

## License

This project is licensed under the MIT License.