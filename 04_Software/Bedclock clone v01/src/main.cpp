#include <Arduino.h>

#define VERSION "1.12"

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
#include "b_Icons.h"        // Icons to show on screen
#include "c_Log.h"          // Logfile for weather data
#include "d_Eventlog.h"     // Event loggers
#include "e_Light.h"        // Controlling the light
#include "f_WiFi.h"         // Wifi 
#include "g_OTA.h"          // Over the air updates
#include "h_Telegram.h"     // Interaction with Telegram
#include "i_Display.h"      // OLED display
#include "j_Weather.h"      // Communication with OpenWeatherMap API

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
  setupTelegram();

  Serial.println( "Bedlight started" );
  addToEventLogfile( "Bedlight started" );

  getWeather();
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
    data.screen = scnWeather2; 
    data.screenBacktoMainTimer.interval=4000;
    data.screenBacktoMainTimer.reset();
    data.updateScreen = true; 
    data.backlightTimer.reset();
    data.backLightOn = true;
  };

  if( keyTopCounter == 3 ) { 
    data.switchLight();    
    data.backlightTimer.reset();
    data.backLightOn = true;
    Serial.printf( "Light switched %s\n", data.lightOn ? "on" : "off" );
  };

  if( keyRightCounter == 3 ) { 
    data.screen = scnWeather1; 
    data.screenBacktoMainTimer.interval=2000;
    data.screenBacktoMainTimer.reset();
    data.updateScreen = true; 
    data.backlightTimer.reset();
    data.backLightOn = true;
  };

  // Tell the light to take the next step
  if( data.lightStepTimer.lapsed() ) {
    loopLight();
    //if( !data.R.ready() ) Serial.printf("Red ready: %s value %.3f target %.3f\n", data.R.ready() ? "Y" : "N", data.R.getValue(), data.R.getTarget() );
  }  

  // Switch the light off if the timer elapses
  if ( data.lightOn and (data.switchLightOffTimer.lapsed() ) ) data.setLightOff();

  // Retrieve weather every now and then so it is available upon request
  if ( data.refreshWeatherTimer.lapsed() ) {
    data.weatherUpdated = false;
    getWeather();
  }

  // Switch back to main screen after time has passed
  if( data.screenBacktoMainTimer.lapsed() and ( data.screen != scnMain ) ) {
    resetScreen(); // Completely reset the screen, since sometimes it switches off
    data.screen = scnMain;
    data.updateScreen = true;
  };

  // Switch of the backlight after some time
  static screenBrightness_t previousScreenBrightness = sb5;
  if( data.backLightOn and data.backlightTimer.lapsed() ) {
    data.backLightOn = false;
    Serial.println("Switching off backlight");
    setScreenBrightness(sb0);
    u8g2.clear(); // Just to be sure, clear screen
    previousScreenBrightness = sb0;
  }

  // Change screen brightness if needed
  if(data.backLightOn) {
    // Backlight is on, set the intensity according to the setting
    if( data.screenBrightness != previousScreenBrightness ) {
      previousScreenBrightness = data.screenBrightness;
      setScreenBrightness(data.screenBrightness);
    };
  }

  // Check if the clock needs an update
  time_t rawtime;
  struct tm * timeinfo;
  static int prev_min = -1;
  static int prev_hour = -1;
  static bool firstClockUpdate = true;

  time( &rawtime );
  timeinfo = localtime( &rawtime );
  snprintf(data.displayClock, sizeof( data.displayClock ), "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min );

  if(data.backLightOn) {    
    if( firstClockUpdate or ( ( timeinfo->tm_year > 80 ) and ( timeinfo->tm_min != prev_min ) ) ) {
      firstClockUpdate = false;
      prev_min = timeinfo->tm_min;

      // Before 15:00, display today's maximum temperature, after 15:00 switch to tomorrows
      if( timeinfo->tm_hour < 15 ) {
        data.maxDisplayTemp =  (int) (data.maxDayTemp+0.5);
      } else {
        data.maxDisplayTemp =  (int) (data.maxTomorrowTemp+0.5);
      }

      if( data.screen == scnMain ) data.updateScreen = true;
    }
  }

  // Automatically reset the daily weather retrieval counter
  if( (timeinfo->tm_hour != prev_hour) && ( timeinfo->tm_hour == 0 ) ) {
    char item[80];
    snprintf( item, sizeof(item), "%d calls to weather API made. Resetting counter", data.weatherRetrievalCounter );
    addToEventLogfile( item );
    data.weatherRetrievalCounter = 0;
  }
  prev_hour = timeinfo->tm_hour;

  // Only call loopDisplay if screen update is requested
  if( data.updateScreen ) loopDisplay();    

  // Check if wifi timer has lapsed
  if( data.loopWifiTimer.lapsed() ) {
    loopWifi(); // Reconnect WiFi if needed
    loopOTA();  // Update software if needed
  };

  // Handle Telegram messages not already done by the handlers
  loopTelegram();

  // Periodically save settings
  if ( data.saveSettingsTimer.lapsed() ) {
    if( data.settingsChanged ) { 
      data.saveSettings(SPIFFS, SETTINGS_FILE, SETTINGS_TEMP);
      listDir(SPIFFS, "/", 6);
    }
  }  
}
