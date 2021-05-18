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

enum mainState_t      {msClock=0, msMenu};
enum powerState_t     {psTimer=0, psOff};
enum lightColor_t     {lcWhite=0, lcYellow, lcOrange, lcRed};
enum lightIntensity_t {li25=0, li35, li50, li70, li100};
enum timerDuration_t  {td03=0, td05, td10, td20, tdOff};

class tData {
  public:

    // Main menu
    mainState_t      mainState;
    powerState_t     powerState;
    lightColor_t     lightColor;
    lightIntensity_t lightIntensity;
    timerDuration_t  timerDuration;

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
    unsigned long lightAlive;
    unsigned long screenAlive;
    
    tData() {
      mainState       = msClock;
      powerState      = psTimer;
      lightColor      = lcWhite;
      lightIntensity  = li25;
      timerDuration   = td03;

      syncTime = false;
      timezone="Europe/Amsterdam";
      timeZoneOffset=3600;
      timeDSToffset=0;

      lightAlive=0;
      screenAlive=0;
    };

};

struct tAccessPoint {
  char ssid[64];
  char password[64];
  char timezone[64];
};

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
};


// ======== GLOBAL VARIABLES ============= 
tConfig config; // Configuration data, to be stored as JSON file in SPIFFS
tData data;
portMUX_TYPE dataAccessMux = portMUX_INITIALIZER_UNLOCKED;

#endif // DATA_H
