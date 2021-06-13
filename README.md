# BedClock

## Functions

Small ESP driven bed clock with a small OLED screen, three capacitive buttons and 16 colour LEDs on top which can be used as a reading light.

The bed clock has three different screens:
* Screen one displays time, current outside temperature and a short term forecast of precipitation
* Screen two displays the time and a random piece of advice
* Screen three is a black screen, if the amount of light is annoying

The top button is used to switch the light on and off
The left button is a 'next' button, moving through screens or options
The right button is a 'select' button, entering  the settings menu or selecting the current setting

Through the menu structure, the following settings can be modified:

* Brightness: changes the brightness level of the lights
* Color: changes the color accent of the lights
* Timer: changes the sleep timer, after which the lights are powered off
* Screen: changes the bightness of the OLED screen

## Internals

All security sensitive data is stored in the JSON file 

...\BedClock\04 Software\NightLight\data\config.jsn

This file is not provided in the github repository. Instead, a template file config template.jsn is provided, which can be copied to config.jsn, after which personal security info can be entered.

The device connects to a WiFi access point and synchronises time with a time server. Multiple access points can be entered, the device connect to any available access point of which the data are known.

For each access point, the timezone and the GPS coordinates are given, so depending on the access point, the location is known. With this location, the time, correction for daylight saving time and local weather report can be retrieved.

The offset for the timezone and the offset for daylight saving time are retrieved from 
Weather is retrieved from api.openweathermap.org. For this service, an API-key must be provided. The API key is free after registration. From this server, the following data is retrieved:
* name of the timezone
* time correction of the timezone (including daylight saving time)
* the current outside temperature
* expected precipitation per minute, for the next hour
* sunrise and sunset (used to automatically correct OLED screen brightness)

Quotes (or rather: random pieces of advice) are retrieved from api.adviceslip.com. It may be more interesting to retrieve quotes, but these are typically too long to fit the small screen.


## Configuration

The JSON file 

...\BedClock\04 Software\NightLight\data\config.jsn

Has the following content:

{
    "AccessPoints": [
        { "SSID": "ssid1", "password": "pwd1", "timezone" : "Europe/Amsterdam", "lat": x.xxx, "lon": y.yyy },
        { "SSID": "ssid2", "password": "pwd1", "timezone" : "Europe/Amsterdam", "lat": x.xxx, "lon": y.yyy },
        { "SSID": "ssid3", "password": "pwd1", "timezone" : "Europe/Amsterdam", "lat": x.xxx, "lon": y.yyy } ],
    "openweathermapAPIkey": "abcdefghijklmnopqrst"
}





..\BedClock\04 Software\NightLight\data\config.jsn

was removed from the repository, since it contained sensitive password info and the api key to the weather api.
Please copy 

..\BedClock\04 Software\NightLight\data\config template.jsn

to config.jsn and add the missing password and api key info. Then upload the datafolder to the ESP32.
