# ESP32-S3 Now Playing Display

An animated now-playing text display built using an ESP32-S3 and a 128×64 OLED screen.

## Features

- Animated text display
- Blinking heart icon
- Moving sparkle animation
- Music note icon
- Progress bar
- Customizable text

## Components

- ESP32-S3
- 128×64 OLED display (SSD1306)
- Jumper wires

## Libraries

Install these libraries in the Arduino IDE:

- Adafruit GFX Library
- Adafruit SSD1306
- Wire

## Wiring

| OLED | ESP32-S3 |
|------|----------|
| VCC  | 3.3V     |
| GND  | GND      |
| SDA  | GPIO 8   |
| SCL  | GPIO 9   |

## How to Use

1. Connect the OLED display to the ESP32-S3.
2. Install the required libraries.
3. Open `esp32_s3_now_playing.ino` in the Arduino IDE.
4. Replace the sample text inside `textList[]` with your own text.
5. Select your ESP32-S3 board and port.
6. Upload the code.

## Customization

You can change the text duration here:

```cpp
const unsigned long TEXT_DURATION = 800;
