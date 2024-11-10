# BedClock
This is a small ESP driven bed clock annex reading light. The device can be controlled using three touch buttons.

<p align="center">
    <img src="02 User interface/Blender render/User manual.png" alt="drawing" width="800"/>
</p>

Normally, the display is empty, because even at the least intense setting the OLED display produces too much light to sleep well.

After one press of the left button, the time is displayed.

On subsequent presses of the left button, the user toggles through the following settings:
* LED intensity
* LED color
* Timer that switches off the LED lights
* Intensity of the OLED display

With the right button, the selected setting can be changed.

## Hardware

This is a list of all items:

| Item | Count | Description                              | Print time | Price March 2023 |
|:----:|:-----:|------------------------------------------|------------|-----------------:|
|   1  |   1   | ESP32 MH-ET Live Minikit                 |            | € 3.63           |
|   2  |   1   | 4x4 Led grid WS2812B                     |            | € 1.00           |
|   3  |   1   | 0.91" OLED 128x32 SSD1306                |            | € 1.53           |
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

The 4x4 LED grid is mounted on top of the LED bridge and then placed over the ESP32.

The light cover 12 is used to cover the onboard LED of the ESP32, since it will shine through the top part if it is not covered.
<br>
<br>

## Software

The original Arduino based software is located in `...\BedClock\04_Software`. This project was used to learn ESP-IDF programming under PlatformIO.

The software projects `BedClock_IDF_V01` until `BedClock_IDF_V12` are demonstrator projects to port parts of the functionality to ESP-IDF.

### Different directories and sub-projects

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

### Files in the `Bedclock_IDF_V12` main directory

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

The `config.h` file only requires three `#defines`:
```c
#define WIFI_SSID    "xxxxxxxxx"
#define WIFI_PASS    "yyyyyyyy"
#define TIME_ZONE    "CET-1CEST,M3.5.0,M10.5.0/3"
```

The code for the local timezone can be found in https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

In `hp_stepping_float.h`, the `hp_stepping_interpolation_t` type is an enumerator that defines the move path between origin and target in the following way:

<p align="center">
    <img src="02 User interface/Move profiles/Move.png" alt="drawing" width="800"/>
</p>

### Issues

The OLED display sometimes stalled, not showing anything anymore. The clock frequency of I2C communication was reduced from 400kHz to 370kHz, since, according to the Salomon specification of the SSD1306, a frequency of up to 407kHz is tolerated, so there is not much headroom at 400kHz. This seems to work. As a fallback, presing the left and right button also resets the screen. But since a hardware reset pin is not provided, only a software reset can be carried out, and it is uncertain if this works.
