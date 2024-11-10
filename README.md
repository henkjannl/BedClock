# BedClock
This is a small ESP driven bed clock annex reading light. The device can be controlled using three touch buttons. Settings such as the light timer, brightness and color and screen intensity can be controlled via Telegram.

<p align="center">
    <img src="02 User interface/Blender render/User manual.png" alt="drawing" width="800"/>
</p>

Normally, the time is displayed on the screen, together with an icon representing the weather prediction for the day

The device has three touch buttons:
* The top button is used to toggle the bedlight on and off. If the bedlight is switched on, it will automatically switch of after the timer expires. The timer can be set in the Telegram menu.
* Upon touching the left button, the current outside temperature, the daily maximum temperature and the outside relative humidity are displayed. After 15:00, the daily maximum temperature of the next day is displayed instead.
* Upon touching the right button, the outside temperature is displayed, together with a graph showin the local precipitation for each minute of the next hour.

After some time, the display switches back to the time.

In Telegram, the light can be switched on or off, and the color, timer and intensity of the bedlight can be set.
Under settings, the intensity of the OLED screen can be set. The settings are saved if power is lost.
Also under settings, a logfile with events can be retrieved for debugging purposes, and a logfile with weather data can be retrieved.


## Hardware

This is a list of all items:

| Item | Count | Description                              | Print time | Price March 2023 |
|:----:|:-----:|------------------------------------------|------------|-----------------:|
|   1  |   1   | ESP32 MH-ET Live Minikit                 |            | € 3.63           |
|   2  |   1   | 4x4 Led grid WS2812B                     |            | € 1.00           |
|   3  |   1   | 0.91" OLED 128x64 #12832 SSD1306         |            | € 1.53           |
|   4  |   1   | Electrical wire AWG 30                   |            | € 1.68           |
|   5  |   3   | Hexagon head button screw ISO7380-1 M3x8 |            | € 1.45           |
|   6  |   3   | Hexagon nut ISO4032 M3 h=2.4             |            | € 1.68           |
|   7  |   4   | Self tapping screw M2.3x12               |            | € 2.33           |
|   8  |   2   | Self tapping screw M2.3x6                |            | € 1.85           |
|   9  |   1   | Bottom part                              | 1h27       | € 0.50           |
|  10  |   1   | Top part (transparent filament)          | 1h14       | € 0.42           |
|  11  |   1   | LED bridge                               | 0h34       | € 0.14           |
|  12  |   1   | Light cover                              | 0h05       | € 0.02           |
|      |       | **Total**                                | **3h20**   | **€ 16.23**      |

Items 1..8 were ordered at AliExpress through the following hyperlinks:

| Item | Count | Order                                                 |
|:----:|:-----:|-------------------------------------------------------|
|   1  |   1   | https://www.aliexpress.com/item/32858054775.html      |
|   2  |   1   | https://www.aliexpress.com/item/32963152993.html      |
|   3  |   1   | https://www.aliexpress.com/item/32788923016.html      |
|   4  |   1   | https://www.aliexpress.com/item/33030965448.html      |
|   5  |   3   | https://www.aliexpress.com/item/32970195293.html      |
|   6  |   3   | https://www.aliexpress.com/item/1005003780212966.html |
|   7  |   4   | https://www.aliexpress.com/item/1005003981975435.html |
|   8  |   2   | https://www.aliexpress.com/item/1005003981975435.html |

It can be beneficial to go for an assortment box of stainless steel screws if you have more similar products, like https://www.aliexpress.com/item/1005002526248698.html instead of items 5 and 6. Self tapping screws can also be ordered in assortment boxes.

Items 9..12 were 3D printed. Assumptions for print time and filament cost:
* Printer Prusa Mk3s
* Material PETG
* Layer height 0.2 mm

All devices are connected to the ESP32 by thin wires according to the schematic below:
<br>
<br>

<p align="center">
    <img src="05 Electrical/Electrical.png" alt="drawing" width="500"/>
</p>

<br>
<br>

Mechanical assembly is as follows:

<p align="center">
    <img src="03 Mechanical/Assembly manual/Assembly manual.png" alt="drawing" width="500"/>
</p>

The 4x4 LED grid is screwed on top of the LED bridge and then placed over the ESP32.

The light cover 12 is used to cover the onboard LED of the ESP32, since it will shine through the top part if it is not covered.
<br>
<br>

## Software

### Arduino

Software is located in `../04 Software/Bedlight` and can be compiled using Arduino IDE.

One file, `MyCredentials.h`, was skipped from the repository by `.gitignore`.

It contains the private information and must be created before compiling. This file should have the following content:

```c++
#pragma once

#include <map>

// Telegram token for the Bedlight bot
const char* token =  "xxxxxxxxxx:yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy";

// Telegram user ID for the user to be notified on startup
int64_t userid = 12345678;

// Timezone where the bedlight is located
// https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define localTimezone "CET-1CEST,M3.5.0,M10.5.0/3"

// List of SSID, password combinations for WiFi access points to connect to
std::map< String , String > ACCESS_POINTS {
  {"ssid", "password" },
  {"ssid", "password" },
  {"ssid", "password" }
};

const char* openweathermapAPIkey = "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";

float latitude  = xx.xxxxx;
float longitude =  x.xxxxx;
```

Where:

| variable             | description                                                                |
|:--------------------:|:---------------------------------------------------------------------------|
| token                | Telegram token of the bot controlling the bed light <sup>1)</sup>          |
| userid               | Telegram user ID of the user that receives a welcome message after startup |
| localTimezone        | Timezone where the bedlight is located <sup>2)</sup>                       |
| ssid                 | SSID's of one or more WiFi access points the bedlight can connect to       |
| password             | The corresponding passwords of WiFi access points                           |
| openweathermapAPIkey | an API key to get access to the openweathermap API <sup>3)</sup>           |

<sup>1)</sup> There are many online sources explaining how a new Telegram bot can be created, such as in this [Random Nerd Tutorials Telegram example](https://randomnerdtutorials.com/telegram-control-esp32-esp8266-nodemcu-outputs/).

<sup>2)</sup> The code for the local timezone can be found in https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

<sup>3)</sup> The API key for Openweathermap can be retrieved after making an account at https://openweathermap.org/
 The free plan allows for 1000 daily API calls. Currently, the weather is retrieved every 12 minutes, resulting in 120 daily calls.


### ESP-IDF

This project was also used to learn ESP-IDF programming under PlatformIO.

The software projects BedClock_IDF_V01 until BedClock_IDF_V12 are demonstrator projects to port parts of the functionality to ESP-IDF. The ESP-IDF has been kept much simpler, eliminating Telegram functionality and connection to the weather app, since this was not used often.

More attention is given to not spreading too much light. By nature, OLED displays cannot be dimmed to a very low degree, so in practice the clock produced a lot of light in a dark sleeping room, even with the display at lowest intensity. Therefore, the clock moves out of the field of view when not used for some time. Pressing the left or the right button brings back the clock.

When pressing the left button again, the display toggles through the next settings: LED intensity, LED color, LED timer and display intensity. At each setting, the right button can be used to choose the next value of the setting. Settings are stored in the non volatile storage of the ESP32 and restored at next power up.

The OLED display sometimes stalled, not showing anything anymore. The clock frequency of I2C communication was reduced from 400kHz to 370kHz, since, according to the Salomon specification of the SSD1306, a frequency of up to 407kHz is tolerated, so there is not much headroom at 400kHz. This seems to work. As a fallback, presing the left and right button also resets the screen. But since a hardware reset pin is not provided, only a software reset can be carried out, and it is uncertain if this works.

#### Different directories and sub-projects

| Directory | Purpose |
|---|---|
| `Bedclock_IDF_V01` | Test WS2812 led chain |
| `Bedclock_IDF_V02` | Test SSD1306 OLED screen |
| `Bedclock_IDF_V03` | Test capacitive touch sensors |
| `Bedclock_IDF_V04` | Test WiFi |
| `Bedclock_IDF_V05` | Test time sync with timeserver |
| `Bedclock_IDF_V06` | Port timer object using esp_timer_get_time() / 1000; |
| `Bedclock_IDF_V07` | Test FreeRTOS |
| `Bedclock_IDF_V08` | Implement multiple parallel processes |
| `Bedclock_IDF_V09` | Internal Espressif SSD1306 driver including LVGL |
| `Bedclock_IDF_V10` | First working version of application |
| `Bedclock_IDF_V11` | First working version of pixel buffer |
| `Bedclock_IDF_V12` | First working version with graphics layout |

#### Files in the `Bedclock_IDF_V12` main directory

| File | Content |
|------|---------|
| `main.c` | Main file, includes other files and calls initializers |
| `bl_keyboard.h` | Checks capacitive touch buttons and communicates with display and light |
| `bl_display.h` | Receives signals from keyboard, changes settings and writes current menu state to SSD1306 OLED display |
| `bl_characters.h` | Character definition to display time on OLED display |
| `bl_light.h` | Writes LED info for the LED grid to the rmt encoder |
| `led_strip_encoder.h/c` | Interface to the rmt encoder |
| `bl_time.h` | Connection with WiFi, syncing of clock with timeserver |
| `config.h` | Not submitted to Github - private user data such as router name and password and timezone |
| `bl_common.h` | Global type and veriable declarations, initalisation of message queues, and storage of parameters in the non volatile storage |
| `hp_pixel_font.h/c` | Library that defines a font by creating a number of constant glyps, and that allows writing text to the canvas |
| `hp_pixel_buffer.h/c` | Library that defines a bitmap for display on the OLED display |
| `hp_pixel_canvas.h/c` | Library that allows writing bitmaps to a buffer, and streaming the buffer to the SSD1306 OLED display |
| `hp_stepping_float.h` | Library that plans and executes paths in linear and non-linear ways (see also `hp_stepping_interpolation_t` below). |
| `hp_timer.h` | Library that defines a repeating or single-shot timer with microsecond resolution. |

In `hp_stepping_float.h`, the `hp_stepping_interpolation_t` type is an enumerator that defines the move path between origin and target in the following way:

<p align="center">
    <img src="02 User interface/Move profiles/Move.png" alt="drawing" width="800"/>
</p>
