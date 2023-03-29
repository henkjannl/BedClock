#pragma once

#include "FS.h"
#include "SPIFFS.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <map>
#include <list>

#define LED_COUNT     16
#define LED_PIN       23
#define LED_CHANNEL    0

#define PIN_KEY_LEFT   4
#define PIN_KEY_RIGHT 12
#define PIN_KEY_TOP   14
#define KEY_TRESHOLD  50

//#define DEG_C "\xB0\x43\x00"
const char DEG_C[] = { 0xb0, 0x43, 0x0 };

// For storing settings in SPIFFS
#define SETTINGS_FILE "/settings.jsn"
#define SETTINGS_TEMP "/settings.tmp"

enum lightColor_t       { lcWhite, lcYellow, lcOrange, lcRed };
enum lightBrightness_t  { lb15, lb30, lb50, lb100 };
enum lightOffTimer_t    { lt03, lt05, lt10, lt20 };
enum screenBrightness_t { sb1, sb2, sb3, sb4, sb5 };
enum screen_t           { scnMain, scnWeather1, scnWeather2 };
enum keyboard_t         { kbMain, kbSettings };

struct rgb_t { 
  float R;
  float G;
  float B; 
};

std::map<lightColor_t, rgb_t> COLORS = { 
  { lcWhite,   { 1.000, 1.000, 1.000 } },   
  { lcYellow,  { 1.000, 1.000, 0.500 } },   
  { lcOrange,  { 1.000, 0.750, 0.500 } },   
  { lcRed,     { 1.000, 0.500, 0.500 } }
};

std::map<lightBrightness_t, float> BRIGHTNESSES = { 
  { lb15,  0.15 },
  { lb30,  0.30 },
  { lb50,  0.50 },
  { lb100, 1.00 } };

std::map<lightOffTimer_t, unsigned int> TIMES = { 
  { lt03,  3*60*1000 }, 
  { lt05,  5*60*1000 },
  { lt10, 10*60*1000 },
  { lt20, 20*60*1000 }
};

std::map<lightOffTimer_t, String > TIMES_STR = { 
  { lt03, "3 minutes"  }, 
  { lt05, "5 minutes"  },
  { lt10, "10 minutes" },
  { lt20, "20 minutes" }
};

// Precipitation for the weather 
struct precipitation_t {
  long t;
  float prec;
};

// Smooth light increments. It is the incremental derivative of x-1/(2*pi)*sin(2*pi*x) in the domain x=0..1. The sum is 1. 
std::vector<float> LIGHT_INC = { 0.0008, 0.0056, 0.0148, 0.0274, 0.0422, 0.0578, 0.0726, 0.0852, 0.0944, 0.0992, 0.0992, 
  0.0944, 0.0852, 0.0726, 0.0578, 0.0422, 0.0274, 0.0148, 0.0056, 0.0008};

const float MAX_INTENSITY = 255.0;

// The order in which the LEDs in the 4x4 grid are switched on
const uint16_t SUB_BRIGHTNESS[] = {0x0000, 0x0008, 0x0208, 0x020A, 0x0A0A, 0x2A0A, 0x2A8A, 0x2AAA, 0xAAAA, 
    0xAABA, 0xABBA, 0xABBE, 0xEBBE, 0xEBBF, 0xFBBF, 0xFBFF };

// This class is used to smoothly control the values for the RGB channels of the light
class floatVariable_t {

  public:
    floatVariable_t (){ 
      _value=0;
      _amplitude=0;
      _index=LIGHT_INC.size();
      _ready=true;
    };    
    
    float getValue()  { return _value; };
    float getTarget() { return _target; };

    // Directly set value without steps
    void setValue(float value){ 
      _value=value; 
      _target=value;
      _ready=true;
    };

    // Go to target with motion steps
    void setTarget(float target) { 
      if( abs(target-_target)>0.001) { // New target is significantly different from previous target
        _amplitude=target-_value;
        _target=target;
        _index=0;
        _ready=false;
      };
    };

    // Do next step
    void step() { 
      if(!_ready) { 
        _value+=_amplitude*LIGHT_INC[_index]; 
        _index++; 
        if(_index>LIGHT_INC.size()-1) _ready=true;
      }; // !_ready
    };

    bool ready() { return _ready; };

  private:
    float _value;
    float _target;
    float _amplitude;
    byte _index;
    bool _ready; 
}; // class floatVariable_t

class milliSecTimer {
  public:
    unsigned long previous;
    unsigned long interval;
    bool autoReset;
    
    // Constructor
    milliSecTimer(unsigned long interval, bool autoReset = true) {
      this->previous = millis();
      this->interval = interval;
      this->autoReset = autoReset;
    }
    
    void reset() { previous = millis(); }
      
    bool lapsed() {
      bool result = (millis() - previous >= interval );
      if(result and autoReset) reset();
      return result;
    }
};

// This is the central repository for data shared between all modules, stored in the global 'data' variable at the bottom of this file
class data_t {
  public:
  
    // Status of the light
    bool              lightOn    = false;
    lightColor_t      color      = lcWhite;    // Saved to settings
    lightBrightness_t brightness = lb50;       // Saved to settings
    lightOffTimer_t   timer      = lt03;       // Saved to settings
  
    // Screen brightness
    screenBrightness_t screenBrightness = sb4; // Saved to settings

    // Weather
    bool requestNewWeather = true;
    bool weatherUpdated = false;
    float outsideTemp = -300;
    float maxDayTemp = -300;
    float outsideFeelsLike = -300; 
    int humidity = -1;
    std::list<precipitation_t> precipitation;
    bool precipitationExpected;
    char weatherIcon[12];
    time_t sunrise;
    time_t sunset;

    // Flag to update screen
    bool updateScreen = true;

    // Clock, temperature
    char displayClock[20];

    // Outside temperature display
    char displayTemperature[20];
    
    // Which screen to display
    screen_t screen = scnMain;

    // Need to save settings 
    bool settingsChanged = false;
    
    // Current menu
    keyboard_t menu = kbMain;

    // Individual RBG values for the light
    floatVariable_t R, G, B;

    // A number of timers for all kinds of tasks
    milliSecTimer screenBacktoMainTimer = milliSecTimer(5000);                // Flip screen back to main after 5 seconds, autoreset
    milliSecTimer switchLightOffTimer   = milliSecTimer(TIMES[timer], true);  // Switch the light off
    milliSecTimer lightStepTimer        = milliSecTimer(100, true);           // Change intensity of light in sall steps
    milliSecTimer refreshWeatherTimer   = milliSecTimer(12*60*1000, true);    // Refresh the weather
    milliSecTimer saveSettingsTimer     = milliSecTimer(5*60*1000);           // Save settings if settings were changed
  
    void setColor( lightColor_t color) { 
      this->color = color; 
      settingsChanged = true;
      updateStatus();
    };

    void setTimer( lightOffTimer_t timer ) {
      this->timer = timer;
      // Update the interval of the light timer in case it was changed
      switchLightOffTimer.interval = TIMES[timer];
      settingsChanged = true;
    };

    void setBrightness( lightBrightness_t brightness ) {
      this->brightness = brightness;
      settingsChanged = true;
      updateStatus();
    };

    void setScreenBrightness( screenBrightness_t screenBrightness ) {
      this->screenBrightness = screenBrightness;
      settingsChanged = true;
    };
    
    void updateStatus() {
      if( lightOn ) {
        // If the user has changed bridghtness or color setting
        R.setTarget(MAX_INTENSITY*COLORS[color].R*BRIGHTNESSES[brightness]);
        G.setTarget(MAX_INTENSITY*COLORS[color].G*BRIGHTNESSES[brightness]);
        B.setTarget(MAX_INTENSITY*COLORS[color].B*BRIGHTNESSES[brightness]);
        Serial.printf( "New light target: R: %.3f G: %.3f B:%.3f\n", R.getTarget(), G.getTarget(), B.getTarget() );
      }
      else {
        R.setTarget(0);
        G.setTarget(0);
        B.setTarget(0);
      } // if lightOn
    }; // updateStatus()
  
    void setLightOn() {
      lightOn = true;
      switchLightOffTimer.reset();
      updateStatus();
    }; // setLightOn()
    
    void setLightOff() {
      lightOn = false;
      updateStatus();
    }; // setLightOff()
    
    void switchLight() {
      if( lightOn ) 
        setLightOff(); 
      else
        setLightOn();
      updateStatus();
    }

    void saveSettings(fs::FS &fs, const char * settingsFile, const char * tempFile) {    

      Serial.printf("Saving settings to temporary file %s\n", tempFile);
    
      // Saves the settings to a file
      StaticJsonDocument<1024> doc;
    
      File output = fs.open(tempFile, FILE_WRITE);

      if( output.isDirectory() ){
          Serial.printf("- temporary file %s is directory\n", tempFile);
          return;
      }
      if(!output ){
          Serial.printf("- failed to open temporary file %s for writing\n", tempFile);
          return;
      }

      doc[ "lightColor"         ] = (int) color;    
      doc[ "lightBrightness"    ] = (int) brightness;    
      doc[ "timer"              ] = (int) timer;    
      doc[ "screenBrightness"   ] = (int) screenBrightness;

      if( serializeJson(doc, output) > 0) {
        // Assume success if at least one byte was written
    
        if( fs.remove(settingsFile) ) 
          Serial.printf("Previous settings file %s removed\n", settingsFile);
        else 
          Serial.printf("Error removing previous settings file %s\n", settingsFile);
          
        if( fs.rename(tempFile, settingsFile) )
          Serial.printf("Temporary file %s renamed to %s\n", tempFile, settingsFile);
        else
          Serial.println("Renaming failed");
      }
      else
       Serial.println("Error during serialization");
    
      settingsChanged = false;
    } 

    void loadSettings(fs::FS &fs, const char * settingsFile) {    
      // Retrieve configuration data from the configuration file stored in SPIFFS 
      StaticJsonDocument<1024> doc;
    
      File input = fs.open(settingsFile);
    
      if(!input || input.isDirectory()){
          Serial.printf("- failed to open settings file %s for reading\n", settingsFile);
          return;
      }
      
      DeserializationError error = deserializeJson(doc, input);
      
      if (error) {
        Serial.print(F("deserializeJson() of settingsfile failed: "));
        Serial.println(error.f_str());
        return;
      }

      try { 
        color             = (lightColor_t)       doc[ "lightColor"         ].as<int>(); 
        brightness        = (lightBrightness_t)  doc[ "lightBrightness"    ].as<int>(); 
        timer             = (lightOffTimer_t)    doc[ "timer"              ].as<int>();
        switchLightOffTimer.interval = TIMES[timer];
        screenBrightness  = (screenBrightness_t) doc[ "screenBrightness"   ].as<int>();
      } 
      catch (const std::exception& e) { 
        Serial.println("Exception occured when deserializing JSON file");
        color              = lcWhite;
        brightness         = lb100;   
        timer              = lt10;   
        screenBrightness   = sb5;    
      };
      
      settingsChanged = false;

      updateStatus();

      Serial.println("Settings loaded");  
    };
    
};

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

data_t data;
