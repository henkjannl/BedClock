#pragma once

#include "FS.h"
#include "SPIFFS.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <map>

#define LED_COUNT     16
#define LED_PIN       23
#define LED_CHANNEL    0

#define PIN_KEY_LEFT   4
#define PIN_KEY_RIGHT 12
#define PIN_KEY_TOP   14
#define KEY_TRESHOLD  50

// For storing settings in SPIFFS
#define SETTINGS_FILE "/settings.jsn"
#define SETTINGS_TEMP "/settings.tmp"

enum lightColor_t       { lcWhite, lcYellow, lcOrange, lcRed };
enum lightBrightness_t  { lb15, lb30, lb50, lb100 };
enum lightTimer_t       { lt03, lt05, lt10, lt20 };
enum screenBrightness_t { sb1, sb2, sb3, sb4, sb5 };
enum screen_t           { scnMain, scnWeather };
enum keyboard_t         { kbMain, kbSetings };

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

std::map<lightTimer_t, unsigned int> TIMES = { 
  { lt03,  3*60*1000 }, 
  { lt05,  5*60*1000 },
  { lt10, 10*60*1000 },
  { lt20, 20*60*1000 }
};

std::map<lightTimer_t, String > TIMES_STR = { 
  { lt03, "3 minutes"  }, 
  { lt05, "5 minutes"  },
  { lt10, "10 minutes" },
  { lt20, "20 minutes" }
};

// Smooth light increments. It is the incremental derivative of x-1/(2*pi)*sin(2*pi*x) in the domain x=0..1. The sum is 1. 
std::vector<float> LIGHT_INC = { 0.0008, 0.0056, 0.0148, 0.0274, 0.0422, 0.0578, 0.0726, 0.0852, 0.0944, 0.0992, 0.0992, 
  0.0944, 0.0852, 0.0726, 0.0578, 0.0422, 0.0274, 0.0148, 0.0056, 0.0008};

const float    MAX_INTENSITY = 255.0;

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

class data_t {
  public:
    // Status of the light
    bool              lightOn    = false;
    lightColor_t      color      = lcWhite;
    lightBrightness_t brightness = lb50;
    lightTimer_t      timer      = lt03;
  
    // Screen brightness
    screenBrightness_t screenBrightness = sb4;

    // Which screen to display
    screen_t screen = scnMain;

    // Need to save settings 
    bool settingsChanged = false;
    
    // Current menu
    keyboard_t menu = kbMain;

    floatVariable_t R, G, B;
  
    unsigned long previousTimer;
  
    void UpdateStatus() {
      if( lightOn ) {
        // If the user has changed bridghtess or color sett
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
    }; // UpdateStatus()
  
    void LightOn() {
      lightOn = true;
      previousTimer = millis();
      UpdateStatus();
    }; // LightOn()
    
    void LightOff() {
      lightOn = false;
      UpdateStatus();
    }; // LightOff()
    
    void SwitchLight() {
      lightOn = !lightOn;
      previousTimer = millis();
      UpdateStatus();
    }

    void saveSettings(fs::FS &fs, const char * tempFile, const char * settingsFile) {    
    
      // Saves the settings to a file
      StaticJsonDocument<1024> doc;
    
      File output = fs.open(tempFile, FILE_WRITE);

      doc[ "lightOn"            ] = lightOn;
      doc[ "lightColor"         ] = (int) color;    
      doc[ "lightBrightness"    ] = (int) brightness;    
      doc[ "timer"              ] = (int) timer;    
      doc[ "screenBrightness"   ] = (int) screenBrightness;
        
      if( serializeJson(doc, output) > 0) {
        // Assume success if at least one byte was written
    
        // Remove settings file to temp file and replace settings file
        fs.remove(settingsFile);
        fs.rename(tempFile, settingsFile);
        Serial.printf("Saved settings to '%s'\n", settingsFile);
      }
    
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

      lightOn          = doc[ "lightOn"            ];
      color            = (lightColor_t)       doc[ "lightColor"         ].as<int>();
      brightness       = (lightBrightness_t)  doc[ "lightBrightness"    ].as<int>();
      timer            = (lightTimer_t)       doc[ "timer"              ].as<int>();
      screenBrightness = (screenBrightness_t) doc[ "screenBrightness"   ].as<int>();
    
      settingsChanged = false;

      Serial.println("Settings loaded");  
    };
    
};

data_t data;
