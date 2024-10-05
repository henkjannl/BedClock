#include <Arduino.h>

#define VERSION "1.13"

/* 
1.00 First working version
1.01 Restructured code
     Added precipitation in screen on press of right button
     Saving /loading settings finally works
1.02 Added advice on press of left button
     Most optimal font for advice chosen
     Timers in separate class
1.03 Added hour scale on precipitation graph
     Added no precipitation icon to weather menu
     Increased number of key press events to prevent light in middle of the night     
     Larger font for current outside temperature 
1.04 Weather icons reintroduced
     Fixed missing weather icon
1.05 Removed advice
     Added second weather screen with 'feels like' temperature and relative humidity
1.06 'Feels like' replaced by maximum day temperature
     Time in main screen two pixels lower to match icon better
1.07 Removed multiple tries of weather retrieval, since we reach the 1000 limit otherwise    
     Display message if weather was not retrieved on time
     Center clock if no weather icon available
1.08 Changed timeout for retrieve weather error    
1.09 Added logging
1.10 Added over the air updates
     Reset weatherRetrievalCounter bug fixed
1.11 Larger buffer to pass weather info
     Detailed error message in event log if JSON fails to pass
1.12 Switched to PlatformIO
     Switch off backlight after some time
1.13 Removed Telegram
     Removed weather
     Removed WiFi
     Changed settings 

To do:
  Switch to latest weather API
  Update readme
  Highlight chosen setting in Telegram keyboard
  Allow over the air updates
  Change LIGHT_INC to function in Light.h
*/

/*
MyCredentials.h is in gitignore to keep confidential 
data from github This file should have the following content:

  #include <map>

  // Password to upload software through wireless port
  #define OTApassword "password-x"
  
  // Telegram token for the Bedlight bot
  const char* token =  "aaaaaaaaaa:bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  
  
  // Telegram user ID for the user to be notified on startup
  int64_t userid = 12345678; 
  
  // Timezone where the bedlight is located
  // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
  #define localTimezone "CET-1CEST,M3.5.0,M10.5.0/3"

  // Location, for the weather API  
  float latitude = 52.2;
  float longitude = 6.8;

  const char* openweathermapAPIkey = "";

  // List of SSID, password combinations for WiFi access points to connect to
  std::map<String, String > ACCESS_POINTS { 
    {"SSID-1", "Password-1" }, 
    {"SSID-2", "Password-2" }, 
    {"SSID-3", "Password-3" } };

*/

#define USE_SPIFFS true

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_wifi.h"

#include "MyCredentials.h"  // All modules have access to data in MyCredentials
#include "a_Data.h"         // This file contains all types and the struct 'data' which acts as a central databus
// #include "b_Icons.h"        // Icons to show on screen
// #include "c_Log.h"          // Logfile for weather data
// #include "d_Eventlog.h"     // Event loggers
#include "e_Light.h"        // Controlling the light
#include "f_WiFi.h"         // Wifi 
// #include "g_OTA.h"          // Over the air updates
// #include "h_Telegram.h"     // Interaction with Telegram
#include "i_Display.h"      // OLED display
// #include "j_Weather.h"      // Communication with OpenWeatherMap API

#include <time.h>
#include <vector>
#include <map>

#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true
 
void setup() {
  // Wait for stable power
  delay(500);

  // Initialize serial port
  Serial.begin(115200);
  Serial.printf( "Version: %s\n", VERSION );

  // Initialize display
  setupDisplay();
  
  // Initialize SPIFFS
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
    Serial.println( "SPIFFS failed" );
  } 

  data.loadSettings(SPIFFS, SETTINGS_FILE);

  setupLight();

  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  esp_wifi_set_max_tx_power(20);
  setupWifi();
  // setupOTA();
  // setupTelegram();

  Serial.println( "Bedlight started" );
  addToEventLogfile( "Bedlight started" );

  // getWeather();
}

void switchBacklightOn() {
  data.backlightTimer.reset();
  data.backLightOn = true;
}

void loop() {
  // Check if button is pressed
  static volatile uint32_t keyLeftCounter  = 0;
  static volatile uint32_t keyRightCounter = 0;
  static volatile uint32_t keyTopCounter   = 0;

  if (touchRead(PIN_KEY_LEFT )<KEY_TRESHOLD) keyLeftCounter++;  else keyLeftCounter =0; 
  if (touchRead(PIN_KEY_TOP  )<KEY_TRESHOLD) keyTopCounter++;   else keyTopCounter  =0; 
  if (touchRead(PIN_KEY_RIGHT)<KEY_TRESHOLD) keyRightCounter++; else keyRightCounter=0; 

  if( keyLeftCounter == 3 ) { 
    data.displayed_item++;
    if(data.displayed_item>5) data.displayed_item=0;

    data.screenBacktoMainTimer.reset();
    data.backlightTimer.reset();
    data.updateScreen = true; 
    data.backLightOn = true;
  };

  if( keyTopCounter == 3 ) { 
    data.switchLight();    
    Serial.printf( "Light switched %s\n", data.lightOn ? "on" : "off" );

    if(data.lightOn) {
      data.screenBacktoMainTimer.reset();
      data.backlightTimer.reset();
      data.updateScreen = true; 
      data.backLightOn = true;
    }
  };

  if( keyRightCounter == 3 ) { 
    switch(data.displayed_item) {
      // 0: switch on display, increase to 1 
      // 1: display already on, increase to 2

      case 2: // modify color of light
        data.incColor();
      break;  

      case 3: // modify brightness of light
        data.incBrightness();
      break; 

      case 4: // modify duration of light timer
        data.incTimer();
      break;  

      case 5: // modify intensity of backlight
        data.incScreenBrightness();
      break;  
    }
    data.screenBacktoMainTimer.reset();
    data.backlightTimer.reset();
    data.updateScreen = true; 
    data.backLightOn = true;
  };

  // Tell the light to take the next step
  if( data.lightStepTimer.lapsed() ) loopLight();

  if( data.screenBacktoMainTimer.lapsed() ) {
    // Go back to display time
    data.displayed_item = 0;
    data.updateScreen = true; 
  }

  // Switch the light off if the timer elapses
  if ( data.lightOn and (data.switchLightOffTimer.lapsed() ) ) data.setLightOff();

  // Switch of the backlight after some time
  static screenBrightness_t previousScreenBrightness = sb5;
  if( data.backLightOn and data.backlightTimer.lapsed() ) {
    data.backLightOn = false;
    Serial.println("Switching off backlight..");
    setScreenBrightness(sb0);
    previousScreenBrightness = sb0;
    // data.updateScreen = true; 
  }

  // Change screen brightness if needed
  if(data.backLightOn) {
    // Backlight is on, adjust the intensity according to the setting
    if( data.screenBrightness != previousScreenBrightness ) {
      previousScreenBrightness = data.screenBrightness;
      setScreenBrightness(data.screenBrightness);
      data.updateScreen = true; 
    };
  }

  // Check if wifi timer has lapsed, also try to adjust the clock
  if( data.loopWifiTimer.lapsed() ) {
    loopWifi(); // Reconnect WiFi if needed
    Serial.print(".");
  };

  static bool firstClockUpdate = true;
  time_t rawtime;
  struct tm * timeinfo;
  static int prev_min = -1;

  if(data.backLightOn and data.timeSynched) {    
    time( &rawtime );
    timeinfo = localtime( &rawtime );

    if( firstClockUpdate or (  timeinfo->tm_min != prev_min ) ) {
      firstClockUpdate = false;
      snprintf(data.displayClock, sizeof( data.displayClock ), "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min );
      data.updateScreen = true; 
      prev_min = timeinfo->tm_min;
      Serial.println("Clock string update");
    }
  }

  // Only call loopDisplay if screen update is requested
  if( data.updateScreen and data.backLightOn) loopDisplay();    

  // Periodically save settings
  if ( data.saveSettingsTimer.lapsed() ) {
    if( data.settingsChanged ) { 
      data.saveSettings(SPIFFS, SETTINGS_FILE, SETTINGS_TEMP);
      listDir(SPIFFS, "/", 6);
    }
  }  
}
