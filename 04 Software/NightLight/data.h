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
enum tMenuItem { mainClock, mainTogglePower, mainTimerExpired, 
                 mainBrightness, mainColor, mainTimer, mainBack,
                 brightness25, brightness35, brightness50, brightness70, brightness100, brightnessBack,
                 colorWhite, colorYellow, colorOrange, colorRed, colorBack,
                 timer03, timer05, timer10, timer20, timerOff, timerBack };

enum tButtonClick { btnNext, btnSelect, btnPower, keyboardTimeout };
                 
class tData {
  public:

    // Connection to internet
    bool connected;

    // Timezone and time
    bool syncTime;
    string timezone;
    int timeZoneOffset;
    int timeDSToffset;

    // System info
    UBaseType_t lightHighWaterMark;  // Unused stack for the measurement thread
    UBaseType_t screenHighWaterMark;  // Unused stack for the display thread
    uint32_t lightAlive;
    uint32_t screenAlive;
    uint32_t keyboardAlive;
    uint32_t screenRedrawMillis;
    
    tData() {
      syncTime = false;
      timezone="Europe/Amsterdam";
      timeZoneOffset=3600;
      timeDSToffset=0;

      lightAlive=0;
      screenAlive=0;
      keyboardAlive=0;
    };

}; // tData

struct tAccessPoint {
  char ssid[64];
  char password[64];
  char timezone[64];
}; // tAccesspoint

class tConfig {
  public:
    list<tAccessPoint> AccessPoints;  
    char botToken[64];
    char chatID[64];
  
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
        AccessPoints.push_back(AccessPoint);    
      }

      strlcpy(botToken,doc["BotToken"],sizeof(botToken)); // "xxxxxxxxxx:yyyyyyyyyyyy-zzzzzzzzzzzzzz_qqqqqqq"
      strlcpy(chatID  ,doc["ChatID"]  ,sizeof(chatID  )); // "-xxxxxxxxxx"
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
