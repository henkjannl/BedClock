#ifndef DATA_H
#define DATA_H

#include <Arduino.h>
#include <map>
#include "esp_system.h"
#include "FS.h"
#include "SPIFFS.h"
#include <list>
#include <string>
#include <ArduinoJson.h>

using namespace std;

// ======== DEFINES ================
#define PIN_ESP32_LED             2

// ======== CONSTANTS ================
const char *CONFIG_FILE = "/config.jsn";

const char EMOTICON_ALARM_CLOCK[]         = { 0xe2, 0x8f, 0xb0, 0x00 };
const char EMOTICON_BACK[]                = { 0xf0, 0x9f, 0x94, 0x99, 0x00 };
const char EMOTICON_BATH[]                = { 0xf0, 0x9f, 0x9b, 0x81, 0x00 };
const char EMOTICON_BED[]                 = { 0xf0, 0x9f, 0x9b, 0x8f, 0x00 };
const char EMOTICON_BEETLE[]              = { 0xf0, 0x9f, 0x90, 0x9e, 0x00 };
const char EMOTICON_BULB[]                = { 0xf0, 0x9f, 0x92, 0xa1, 0x00 };
const char EMOTICON_BULLSEYE[]            = { 0xf0, 0x9f, 0x8e, 0xaf, 0x00 };
const char EMOTICON_CALENDAR[]            = { 0xf0, 0x9f, 0x93, 0x85, 0x00 };
const char EMOTICON_CAMERA[]              = { 0xf0, 0x9f, 0x93, 0xb7, 0x00 };
const char EMOTICON_CHECKERED_FLAG[]      = { 0xf0, 0x9f, 0x8f, 0x81, 0x00 };
const char EMOTICON_CHECKMARK[]           = { 0xe2, 0x9c, 0x85, 0x00 };
const char EMOTICON_CLIPBOARD[]           = { 0xf0, 0x9f, 0x93, 0x8b, 0x00 };
const char EMOTICON_CLOCK[]               = { 0xe2, 0x8f, 0xb0, 0x00 };
const char EMOTICON_CROSSMARK[]           = { 0xe2, 0x9d, 0x8c, 0x00 };
const char EMOTICON_DANCER[]              = { 0xf0, 0x9f, 0x92, 0x83, 0xf0, 0x9f, 0x8f, 0xbb, 0x00 };
const char EMOTICON_DOT_BLACK[]           = { 0xe2, 0x9a, 0xab, 0xef, 0xb8, 0x8f, 0x00 };
const char EMOTICON_DOT_BLUE[]            = { 0xf0, 0x9f, 0x94, 0xb5, 0x00 };
const char EMOTICON_DOT_GREEN[]           = { 0xf0, 0x9f, 0x94, 0xb5, 0x00 };
const char EMOTICON_DOT_YELLOW[]          = { 0xf0, 0x9f, 0x9f, 0xa1, 0x00 };
const char EMOTICON_DOWN_ARROW[]          = { 0xe2, 0xac, 0x87, 0xef, 0xb8, 0x8f, 0x00 };
const char EMOTICON_EARTH[]               = { 0xf0, 0x9f, 0x8c, 0x8d, 0x00 };
const char EMOTICON_FLAME[]               = { 0xf0, 0x9f, 0x94, 0xa5, 0x00 };
const char EMOTICON_FOOTSTEPS[]           = { 0xf0, 0x9f, 0x91, 0xa3, 0x00 };
const char EMOTICON_GEAR[]                = { 0xe2, 0x9a, 0x99, 0xef, 0xb8, 0x8f, 0x00 };
const char EMOTICON_GLASSES[]             = { 0xf0, 0x9f, 0x91, 0x93, 0x00 };
const char EMOTICON_HAMMER_WRENCH[]       = { 0xf0, 0x9f, 0x9b, 0xa0, 0x00 };
const char EMOTICON_HOURGLASS[]           = { 0xe2, 0x8c, 0x9b, 0xef, 0xb8, 0x8f, 0x00 };
const char EMOTICON_HOUSE[]               = { 0xf0, 0x9f, 0x8f, 0xa0, 0x00 };
const char EMOTICON_ISLAND[]              = { 0xf0, 0x9f, 0x8f, 0x96, 0x00 };
const char EMOTICON_LAMP[]                = { 0xf0, 0x9f, 0x92, 0xa1, 0x00 };
const char EMOTICON_LEVEL_SLIDER[]        = { 0xf0, 0x9f, 0x8e, 0x9a, 0x00 };
const char EMOTICON_LIFEBUOY[]            = { 0xf0, 0x9f, 0x9b, 0x9f, 0x00 };
const char EMOTICON_MAGIC_STICK[]         = { 0xf0, 0x9f, 0xaa, 0x84, 0x00 };
const char EMOTICON_MINUS[]               = { 0xe2, 0x9e, 0x96, 0x00 };
const char EMOTICON_OFFICE[]              = { 0xf0, 0x9f, 0x8f, 0xa2, 0x00 };
const char EMOTICON_PIN[]                 = { 0xf0, 0x9f, 0x93, 0x8d, 0x00 };
const char EMOTICON_PLUS[]                = { 0xe2, 0x9e, 0x95, 0x00 };
const char EMOTICON_POINTING_FINGER[]     = { 0xf0, 0x9f, 0x91, 0x89, 0x00 };
const char EMOTICON_RED_DOT[]             = { 0xf0, 0x9f, 0x94, 0xb4, 0x00 };
const char EMOTICON_RED_QUESTION_MARK[]   = { 0xe2, 0x9d, 0x93, 0x00 };
const char EMOTICON_RUNNER[]              = { 0xf0, 0x9f, 0x8f, 0x83, 0x00 };
const char EMOTICON_SATTELITE_DISH[]      = { 0xf0, 0x9f, 0x93, 0xa1, 0x00 };
const char EMOTICON_SHOWER[]              = { 0xf0, 0x9f, 0x9a, 0xbf, 0x00 };
const char EMOTICON_SMILEY[]              = { 0xf0, 0x9f, 0x98, 0x80, 0x00 };
const char EMOTICON_SNOWFLAKE[]           = { 0xe2, 0x9d, 0x84, 0xef, 0xb8, 0x8f, 0x00 };
const char EMOTICON_SPIDERWEB[]           = { 0xf0, 0x9f, 0x95, 0xb8, 0x00 };
const char EMOTICON_SPEECH_BALLOON[]      = { 0xf0, 0x9f, 0x92, 0xac, 0x00 };
const char EMOTICON_STHETOSCOPE[]         = { 0xf0, 0x9f, 0xa9, 0xba, 0x00 };
const char EMOTICON_STOPWATCH[]           = { 0xe2, 0x8f, 0xb1, 0x00 };
const char EMOTICON_SUN[]                 = { 0xf0, 0x9f, 0x8c, 0x9e, 0x00 };
const char EMOTICON_THERMOMETER[]         = { 0xf0, 0x9f, 0x8c, 0xa1, 0x00 };
const char EMOTICON_TRIANGLE_DOWN[]       = { 0xf0, 0x9f, 0x94, 0xbb, 0x00 };
const char EMOTICON_TRIANGLE_UP[]         = { 0xf0, 0x9f, 0x94, 0xba, 0x00 };
const char EMOTICON_UP_ARROW[]            = { 0xe2, 0xac, 0x86, 0xef, 0xb8, 0x8f, 0x00 };
const char EMOTICON_WARNING[]             = { 0xe2, 0x9a, 0xa0, 0xef, 0xb8, 0x8f, 0x00 };

// ======== TYPES ================

enum screen_t { scnMain };

enum command_t { 
  // Commands to control the light
  cmdLightOn, cmdLightOff, cmdLightToggle,
  cmdDuration03, cmdDuration05, cmdDuration10, cmdDuration20, cmdDurationForever,
  cmdColorWhite, cmdColorYellow, cmdColorAmber, cmdColorRed,
  cmdBrightness25, cmdBrightness40, cmdBrightness60, cmdBrightness100,
  cmdLightTimerExpired,
  
  cmdEndOfLightCommands,
  
  // Commands only for the keyboard
  cmdButtonLeft, cmdButtonRight,

  // Commands only for Telegram
  cmdStartTelegram,
  cmdUpdateSoftware,
  cmdRedrawScreen,
  
  cmdCommandNotRecognized };

// Button faces for commands that are available in Telegram
struct labelCallback_t {
  String label;            // label can change depending on the context
  const String callback;   // callback strings must remain the same
};

// Commands that have an associated button in Telegram
std::map<command_t, labelCallback_t> telegramButtons = {
  { cmdLightOn,         { String(EMOTICON_DOT_YELLOW) + " Light on",  "/cmdLightOn" } },
  { cmdLightOff,        { String(EMOTICON_DOT_BLACK)  + " Light off", "/cmdLightOff" } },
  { cmdDuration03,      { "3 minutes",  "/cmdDuration03"      } },
  { cmdDuration05,      { "5 minutes",  "/cmdDuration05"      } },
  { cmdDuration10,      { "10 minutes", "/cmdDuration10"      } },
  { cmdDuration20,      { "20 minutes", "/cmdDuration20"      } },
  { cmdDurationForever, { "Timer off",  "/cmdDurationForever" } },
  { cmdColorWhite,      { "White",      "/cmdColorWhite"      } },
  { cmdColorYellow,     { "Yellow",     "/cmdColorYellow"     } },
  { cmdColorAmber,      { "Amber",      "/cmdColorAmber"      } },
  { cmdColorRed,        { "Red",        "/cmdColorRed"        } },
  { cmdBrightness25,    { "25%",        "/cmdBrightness25"    } },
  { cmdBrightness40,    { "40%",        "/cmdBrightness40"    } },
  { cmdBrightness60,    { "60%",        "/cmdBrightness60"    } },
  { cmdBrightness100,   { "100%",       "/cmdBrightness100"   } },
};  

struct precipitation_t {
  long t;
  float prec;
};

enum dataQuality_t { dqUnavailable, dqRefreshed, dqDisplayed };

class data_t {
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
    dataQuality_t weatherAvailable;
    float outsideTemp;
    list<precipitation_t> precipitation;
    char weatherIcon[12];
    // Todo: deal with time to adjust screen contrast
    //https://en.cppreference.com/w/cpp/chrono/c/time
    time_t sunrise;
    time_t sunset;

    // Quote
    bool requestQuote;
    dataQuality_t quoteAvailable;
    string quote;
  
    // System info
    UBaseType_t lightHighWaterMark;   // Unused stack for the measurement thread
    UBaseType_t screenHighWaterMark;  // Unused stack for the display thread
    UBaseType_t weatherHighWaterMark; // Unused stack for the weather thread
    UBaseType_t quoteHighWaterMark; // Unused stack for the weather thread
    
    uint32_t lightAlive;
    uint32_t screenAlive;
    uint32_t keyboardAlive;
    uint32_t weatherAlive;
    uint32_t quoteAlive;

    uint32_t screenRedrawMillis;
    
    data_t() {
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

}; // data_t

struct accessPoint_t {
  char ssid[64];
  char password[64];
  char timezone[64];
  float lat, lon;
}; // accesspoint_t

class config_t {
  public:
    list<accessPoint_t> AccessPoints;  
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
        accessPoint_t AccessPoint;
        strlcpy(AccessPoint.ssid     ,elem["SSID"],    sizeof(AccessPoint.ssid    ));
        strlcpy(AccessPoint.password ,elem["password"],sizeof(AccessPoint.password));
        strlcpy(AccessPoint.timezone ,elem["timezone"],sizeof(AccessPoint.timezone));         
        AccessPoint.lat = elem["lat"]; // 52.25319, 56.25319
        AccessPoint.lon = elem["lon"]; // 6.78546, 6.78546
        AccessPoints.push_back(AccessPoint);    
      }

      strlcpy(botToken,doc["BotToken"],sizeof(botToken)); // "xxxxxxxxxx:yyyyyyyyyyyy-zzzzzzzzzzzzzz_qqqqqqq"
      strlcpy(chatID  ,doc["ChatID"]  ,sizeof(chatID  )); // "-xxxxxxxxxx"
      strlcpy(openweathermapAPIkey,doc["openweathermapAPIkey"]  ,sizeof(openweathermapAPIkey) ); 
      Serial.println("Config loaded");
    }

    void Save() {
      // To do: loop over access points
    }
}; // config_t


// ======== HELPER FUNCTIONS ============= 
// Helper function that generates a string with the current time, used under a message sent to the user
String currentTime() {
  time_t now;
  time(&now);
  struct tm * local;
  local = localtime(&now);
  char buffer[20];
  snprintf(buffer, 20, "%02d:%02d:%02d", local->tm_hour, local->tm_min, local->tm_sec);
  return String(buffer);
};

String convertToHexString( String input ) {
  String result = "{ ";
  for( int i=0; i<input.length(); i++ ) {
    result += String("0x") + String( (int) input[i], HEX ) + ", ";
  }
  result+="0x00 };";
  result.toLowerCase();
  return result;
};

void sendCommandToQueue( command_t command, QueueHandle_t queue ) {
  xQueueSendToBack( queue, &command, 0 );
};

// ======== GLOBAL VARIABLES ============= 
config_t config; // Configuration data, to be stored as JSON file in SPIFFS
data_t data;

portMUX_TYPE dataAccessMux = portMUX_INITIALIZER_UNLOCKED;

QueueHandle_t screenQueue   = xQueueCreate( 20, sizeof(command_t) );
QueueHandle_t lightQueue    = xQueueCreate( 20, sizeof(command_t) );
QueueHandle_t displayQueue  = xQueueCreate( 20, sizeof(command_t) );

#endif // DATA_H
