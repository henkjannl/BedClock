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

This is a list of all items (e.g., at AliExpress), including fasteners:

| Item | Count | Description                              | Order                                                 | Print time | Price March 2023 |
|:----:|:-----:|------------------------------------------|-------------------------------------------------------|------------|-----------------:|
|   1  |   1   | ESP32 MH-ET Live Minikit                 | https://www.aliexpress.com/item/32858054775.html      |            | € 3.63           |
|   2  |   1   | 4x4 Led grid WS2812B                     | https://www.aliexpress.com/item/32963152993.html      |            | € 1.00           |
|   3  |   1   | 0.91" OLED 128x64 #12832 SSD1306         | https://www.aliexpress.com/item/32788923016.html      |            | € 1.53           |
|   4  |   1   | Electrical wire AWG 30                   | https://www.aliexpress.com/item/33030965448.html      |            | € 1.68           | 
|   5  |   3   | Hexagon head button screw ISO7380-1 M3x8 | https://www.aliexpress.com/item/32970195293.html      |            | € 1.45           |
|   6  |   3   | Hexagon nut ISO4032 M3 h=2.4             | https://www.aliexpress.com/item/1005003780212966.html |            | € 1.68           |
|   7  |   4   | Self tapping screw M2.3x12               | https://www.aliexpress.com/item/1005003981975435.html |            | € 2.33           |
|   8  |   2   | Self tapping screw M2.3x6                | https://www.aliexpress.com/item/1005003981975435.html |            | € 1.85           |
|   9  |   1   | Bottom part                              |                                                       | 1h27       | € 0.50           |
|  10  |   1   | Top part (transparent filament)          |                                                       | 1h14       | € 0.42           |
|  11  |   1   | LED bridge                               |                                                       | 0h34       | € 0.14           |
|  12  |   1   | Light cover                              |                                                       | 0h05       | € 0.02           |
|      |       | **Total**                                |                                                       | **3h20**   | **€ 16.23**      | 

Items 1..8 were ordered at AliExpress. It can be beneficial to go for an assortment box of stainless steel screws if you have more similar products, like https://www.aliexpress.com/item/1005002526248698.html instead of items 5 and 6. Self tapping screws can also be ordered in assortment boxes.

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
 

