#ifndef DATA_H
#define DATA_H

#include <Arduino.h>
#include "esp_system.h"
#include "FS.h"
#include "SPIFFS.h"
#include "time.h"
#include <list>
#include <string>
#include <WiFi.h>
#include <ArduinoJson.h>

using namespace std;

// ======== CONSTANTS ================
const char *CONFIG_FILE = "/config.jsn";

// ======== TYPES ================

// All commands displayed on screen and sent to light
enum tMenuItem { mainScreen, mainFull, mainNight, mainEmpty, 
                 mainPowerOn, mainPowerOff, mainTogglePower, mainTimerExpired, 
                 mainBrightness, mainColor, mainTimer, mainScreenContrast, mainBack,
                 brightness25, brightness35, brightness50, brightness70, brightness100, brightnessBack,
                 colorWhite, colorYellow, colorOrange, colorRed, colorBack,
                 timer03, timer05, timer10, timer20, timerOff, timerBack, 
                 screenContrastAuto, screenContrast10, screenContrast35, screenContrast100, screenContrastBack };

char *menuItem[] = { "mainScreen", "mainFull", "mainNight", "mainEmpty", 
                     "mainPowerOn", "mainPowerOff", "mainTogglePower", "mainTimerExpired", 
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
    time_t sunrise;
    time_t sunset;

    // Quote
    bool requestQuote;
    tDataQuality quoteAvailable;
    string quote;
  
    // System info
    UBaseType_t lightHighWaterMark;   // Unused stack for the measurement thread
    UBaseType_t screenHighWaterMark;  // Unused stack for the display thread
    UBaseType_t weatherHighWaterMark; // Unused stack for the weather thread
    UBaseType_t quoteHighWaterMark;   // Unused stack for the weather thread
    
    uint32_t lightAlive;
    uint32_t screenAlive;
    uint32_t keyboardAlive;
    uint32_t weatherAlive;
    uint32_t quoteAlive;

    uint32_t screenRedrawMillis;
    
    tData() {
      connected=false;
      syncTime = false;
      timezone="Europe/Amsterdam";
      timeZoneOffset=3600;
      timeDSToffset=0;
      lat=52.25319;
      lon=6.78546;

      weatherAvailable=dqUnavailable;

      quoteAvailable=dqUnavailable;
      requestQuote=false;

      lightAlive=0;
      screenAlive=0;
      keyboardAlive=0;
      weatherAlive=0;
    };

}; // tData

struct tAccessPoint {
  char ssid[64];
  char password[64];
  float lat, lon;
}; // tAccesspoint

class tConfig {
  public:
    list<tAccessPoint> AccessPoints;  
    string openweathermapAPIkey;
  
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
        AccessPoint.lat = elem["lat"]; // 52.25319, 56.25319
        AccessPoint.lon = elem["lon"]; // 6.78546, 6.78546
        AccessPoints.push_back(AccessPoint);    
      }

      openweathermapAPIkey=doc["openweathermapAPIkey"].as<string>();
    }

    void Save() {
      // To do: loop over access points
    }
}; // tConfig

// ======== GLOBAL VARIABLES ============= 
// Ensure only one process at a time uses wifi
portMUX_TYPE connectionMux = portMUX_INITIALIZER_UNLOCKED;

tConfig config; // Configuration data, to be stored as JSON file in SPIFFS
tData data;
portMUX_TYPE dataAccessMux = portMUX_INITIALIZER_UNLOCKED;

QueueHandle_t keyboardQueue;
QueueHandle_t lightQueue;


// ======== GLOBAL HELPER FUNCTIONS =============
void connectToWiFi() {
  
  /* =========================================================
   *  The code below is way more complex than I'd wish
   *  but somehow I could not make wifiMulti work together 
   *  with FreeRTOS
   *  ======================================================== */
   
  portENTER_CRITICAL(&connectionMux);
  portENTER_CRITICAL(&dataAccessMux);

  for(auto accessPoint : config.AccessPoints) Serial.println(accessPoint.ssid);

  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    for(auto accessPoint : config.AccessPoints) {
      if( WiFi.SSID(i) == String(accessPoint.ssid) ) {
          Serial.printf("Connecting to %s\n", accessPoint.ssid);  
          WiFi.begin(accessPoint.ssid, accessPoint.password);
          for(int j=0; j<35; j++) {
            if (WiFi.status() != WL_CONNECTED)
              {
                Serial.print(".");
                vTaskDelay(500);
              }
            else {
              Serial.printf("\nWiFi connected to %s.\n", WiFi.SSID());

              // Based on the accesspoint, we know where we are.
              // the weather service can now determine the timezone and daylight saving time
              portENTER_CRITICAL(&dataAccessMux);
              data.lat=accessPoint.lat;
              data.lon=accessPoint.lon;
              portEXIT_CRITICAL(&dataAccessMux);  
              data.connected = true;

              break;
            } // not yet connected
            if(data.connected) break; // break j loop
        } // for j
      } // if ssid==ssid
      if(data.connected) break; // break accessPoint loop
    } // for accessPoint
    if(data.connected) break; // break i loop
  } // for i

  data.connected = (WiFi.status() == WL_CONNECTED);
  portEXIT_CRITICAL(&dataAccessMux);  
  portEXIT_CRITICAL(&connectionMux);
} // connectToWifi


void syncTime() {
  string url;
  
  portENTER_CRITICAL(&connectionMux);

  HTTPClient http;

  url=string("http://worldtimeapi.org/api/timezone/") + data.timezone;
  http.begin(url.c_str()); 
  
  int httpCode = http.GET();
  
  if(httpCode > 0) {    
    if(httpCode == HTTP_CODE_OK) {
      StaticJsonDocument<1024> doc;

      DeserializationError error = deserializeJson(doc, http.getString());

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      portENTER_CRITICAL(&dataAccessMux);
        //const char* abbreviation = doc["abbreviation"]; // "CEST"
        //const char* client_ip = doc["client_ip"]; // "80.100.167.45"
        //const char* datetime = doc["datetime"]; // "2021-04-05T14:28:36.119148+02:00"
        //int day_of_week = doc["day_of_week"]; // 1
        //int day_of_year = doc["day_of_year"]; // 95
        //bool dst = doc["dst"]; // true
        //const char* dst_from = doc["dst_from"]; // "2021-03-28T01:00:00+00:00"
        data.timeDSToffset = doc["dst_offset"]; // 3600
        //const char* dst_until = doc["dst_until"]; // "2021-10-31T01:00:00+00:00"
        data.timeZoneOffset = doc["raw_offset"]; // 3600
        //const char* timezone = doc["timezone"]; // "Europe/Amsterdam"
        //long unixtime = doc["unixtime"]; // 1617625716
        //const char* utc_datetime = doc["utc_datetime"]; // "2021-04-05T12:28:36.119148+00:00"
        //const char* utc_offset = doc["utc_offset"]; // "+02:00"
        //int week_number = doc["week_number"]; // 14
      portEXIT_CRITICAL(&dataAccessMux);  

      configTime(data.timeZoneOffset, data.timeDSToffset, "pool.ntp.org");
    
      portENTER_CRITICAL(&dataAccessMux);
        data.syncTime = true;
      portEXIT_CRITICAL(&dataAccessMux);  
    }
  }
  
  portEXIT_CRITICAL(&connectionMux);
  
} // syncTime


#endif // DATA_H
