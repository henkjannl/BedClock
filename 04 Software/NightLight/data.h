#ifndef DATA_H
#define DATA_H

#include <Arduino.h>
#include "esp_system.h"
#include "FS.h"
#include "SPIFFS.h"
#include <list>
#include <string>
#include <ArduinoJson.h>

using namespace std;

// ======== CONSTANTS ================
const char *CONFIG_FILE = "/config.jsn";

// ======== TYPES ================

// All commands displayed on screen and sent to light
enum tMenuItem { mainScreen, mainFull, mainNight, mainEmpty, 
                 mainPowerOn, mainTogglePower, mainTimerExpired, 
                 mainBrightness, mainColor, mainTimer, mainScreenContrast, mainBack,
                 brightness25, brightness35, brightness50, brightness70, brightness100, brightnessBack,
                 colorWhite, colorYellow, colorOrange, colorRed, colorBack,
                 timer03, timer05, timer10, timer20, timerOff, timerBack, 
                 screenContrastAuto, screenContrast10, screenContrast35, screenContrast100, screenContrastBack };

char *menuItem[] = { "mainScreen", "mainFull", "mainNight", "mainEmpty", 
                     "mainPowerOn", "mainTogglePower", "mainTimerExpired", 
                     "mainBrightness", "mainColor", "mainTimer", "mainScreenContrast", "mainBack",
                     "brightness25", "brightness35", "brightness50", "brightness70", "brightness100", "brightnessBack",
                     "colorWhite", "colorYellow", "colorOrange", "colorRed", "colorBack",
                     "timer03", "timer05", "timer10", "timer20", "timerOff", "timerBack", 
                     "screenContrastAuto", "screenContrast10", "screenContrast35", "screenContrast100", "screenContrastBack" };

enum tButtonClick { btnNext, btnSelect, btnPower, keyboardTimeout };

enum tDataQuality { dqUnavailable, dqRefreshed, dqDisplayed };

struct tPrecipitation {
  long t;
  float prec;
};

class tData {
  public:

    // Connection to internet
    bool connected;

    // Timezone and time
    bool syncTime;
    string timezone;
    int timeZoneOffset;
    int timeDSToffset;
    float lat, lon;

    // Weather info
    bool requestWeather;
    tDataQuality weatherAvailable;
    float outsideTemp;
    list<tPrecipitation> precipitation;
    char weatherIcon[12];
    // Todo: deal with time to adjust screen contrast
    //https://en.cppreference.com/w/cpp/chrono/c/time
    time_t sunrise;
    time_t sunset;
  
    // System info
    UBaseType_t lightHighWaterMark;   // Unused stack for the measurement thread
    UBaseType_t screenHighWaterMark;  // Unused stack for the display thread
    UBaseType_t weatherHighWaterMark; // Unused stack for the weather thread
    
    uint32_t lightAlive;
    uint32_t screenAlive;
    uint32_t keyboardAlive;
    uint32_t weatherAlive;
    uint32_t screenRedrawMillis;
    
    tData() {
      syncTime = false;
      timezone="Europe/Amsterdam";
      timeZoneOffset=3600;
      timeDSToffset=0;
      lat=52.25319;
      lon=6.78546;

      weatherAvailable=dqUnavailable;

      lightAlive=0;
      screenAlive=0;
      keyboardAlive=0;
      weatherAlive=0;
    };

}; // tData

struct tAccessPoint {
  char ssid[64];
  char password[64];
  char timezone[64];
  float lat, lon;
}; // tAccesspoint

class tConfig {
  public:
    list<tAccessPoint> AccessPoints;  
    char botToken[64];
    char chatID[64];
    char openweathermapAPIkey[64];
  
    void load() {    
      StaticJsonDocument<1024> doc;
      File input = SPIFFS.open(CONFIG_FILE);
      DeserializationError error = deserializeJson(doc, input);
      
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      
      for (JsonObject elem : doc["AccessPoints"].as<JsonArray>()) {
        tAccessPoint AccessPoint;
        strlcpy(AccessPoint.ssid     ,elem["SSID"],    sizeof(AccessPoint.ssid    ));
        strlcpy(AccessPoint.password ,elem["password"],sizeof(AccessPoint.password));
        strlcpy(AccessPoint.timezone ,elem["timezone"],sizeof(AccessPoint.timezone));         
        AccessPoint.lat = elem["lat"]; // 52.25319, 56.25319
        AccessPoint.lon = elem["lon"]; // 6.78546, 6.78546
        AccessPoints.push_back(AccessPoint);    
      }

      strlcpy(botToken,doc["BotToken"],sizeof(botToken)); // "xxxxxxxxxx:yyyyyyyyyyyy-zzzzzzzzzzzzzz_qqqqqqq"
      strlcpy(chatID  ,doc["ChatID"]  ,sizeof(chatID  )); // "-xxxxxxxxxx"
      strlcpy(openweathermapAPIkey,doc["openweathermapAPIkey"]  ,sizeof(openweathermapAPIkey)); 
    }

    void Save() {
      // To do: loop over access points
    }
}; // tConfig


// ======== GLOBAL VARIABLES ============= 
tConfig config; // Configuration data, to be stored as JSON file in SPIFFS
tData data;
portMUX_TYPE dataAccessMux = portMUX_INITIALIZER_UNLOCKED;

QueueHandle_t keyboardQueue;
QueueHandle_t lightQueue;

#endif // DATA_H
