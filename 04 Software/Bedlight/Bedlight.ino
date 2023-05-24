#define VERSION "1.9"

/* 
1.0 First working version
1.1 Restructured code
    Added precipitation in screen on press of right button
    Saving /loading settings finally works
1.2 Added advice on press of left button
    Most optimal font for advice chosen
    Timers in separate class
1.3 Added hour scale on precipitation graph
    Added no precipitation icon to weather menu
    Increased number of key press events to prevent light in middle of the night     
    Larger font for current outside temperature 
1.4 Weather icons reintroduced
    Fixed missing weather icon
1.5 Removed advice
    Added second weather screen with 'feels like' temperature and relative humidity
1.6 'Feels like' replaced by maximum day temperature
    Time in main screen two pixels lower to match icon better
1.7 Removed multiple tries of weather retrieval, since we reach the 1000 limit otherwise    
    Display message if weather was not retrieved on time
    Center clock if no weather icon available
1.8 Changed timeout for retrieve weather error    
1.9 Added logging

To do:
  Update readme
  Highlight chosen setting in Telegram keyboard
  Allow over the air updates
  Change LIGHT_INC to function in Light.h
*/

/*
MyCredentials.h is in gitignore to keep confidential 
data from github This file should have the following content:

  #include <map>
  
  // Telegram token for the Bedlight bot
  const char* token =  "aaaaaaaaaa:bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  
  
  // Telegram user ID for the user to be notified on startup
  int64_t userid = 12345678; 
  
  // Timezone where the bedlight is located
  // https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
  #define localTimezone "CET-1CEST,M3.5.0,M10.5.0/3"
  
  // List of SSID, password combinations for WiFi access points to connect to
  std::map<String, String > ACCESS_POINTS { 
    {"SSID-1", "Password-1" }, 
    {"SSID-2", "Password-2" }, 
    {"SSID-3", "Password-3" } };

*/

#include "MyCredentials.h"  // All modules have access to data in MyCredentials
#include "a_Data.h"         // This file contains all types and the struct 'data' which acts as a central databus
#include "b_Icons.h"
#include "c_Light.h"
#include "d_WiFi.h"
#include "e_Telegram.h"
#include "f_Display.h"
#include "g_Weather.h"
#include "h_log.h"
#include "i_Eventlog.h"

#include <time.h>
#include <vector>
#include <map>

#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true

 
void setup() {
  Serial.begin(115200);
  setupDisplay();
  
  SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);
  data.loadSettings(SPIFFS, SETTINGS_FILE);
  
  setupLight();
  setupWifi();
  setupTelegram();

  addToEventLogfile( "Bedlight started" );

  getWeather();
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
  };

  if( keyTopCounter == 3 ) { 
    data.switchLight();    
    Serial.printf( "Light switched %s\n", data.lightOn ? "on" : "off" );
  };

  if( keyRightCounter == 3 ) { 
    data.screen = scnWeather1; 
    data.screenBacktoMainTimer.interval=2000;
    data.screenBacktoMainTimer.reset();
    data.updateScreen = true; 
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

  // Change screen brightness if needed
  static screenBrightness_t previousScreenBrightness = sb5;
  if( data.screenBrightness != previousScreenBrightness ) {
    previousScreenBrightness = data.screenBrightness;
    setScreenBrightness(data.screenBrightness);
  };

  // Check if the clock needs an update
  time_t rawtime;
  struct tm * timeinfo;
  static int prev_min = -1;
  static int prev_hour = -1;
  static bool firstClockUpdate = true;
  
  time( &rawtime );
  timeinfo = localtime( &rawtime );
  if( firstClockUpdate or ( ( timeinfo->tm_year > 80 ) and ( timeinfo->tm_min != prev_min ) ) ) {
    firstClockUpdate = false;
    prev_min = timeinfo->tm_min;
    snprintf(data.displayClock, sizeof( data.displayClock ), "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min );

    // Before 15:00, display today's maximum temperature, after 15:00 switch to tomorrows
    if( timeinfo->tm_hour < 15 ) {
      data.maxDisplayTemp =  (int) (data.maxDayTemp+0.5);
    } else {
      data.maxDisplayTemp =  (int) (data.maxTomorrowTemp+0.5);
    }

    if( data.screen == scnMain ) data.updateScreen = true;
  }

  // Automatically reset the daily weather retrieval counter
  if( (timeinfo->tm_hour ==0) && (prev_hour != 0) ) {
    prev_hour = timeinfo->tm_hour;
    char item[80];
    snprintf( item, sizeof(item), "%d calls to weather API made, resetting counter", data.weatherRetrievalCounter );
    addToEventLogfile( item );
    data.weatherRetrievalCounter = 0;
  }

  // Only call loopDisplay if screen update is requested
  if( data.updateScreen ) loopDisplay();    

  // Check if wifi timer has lapsed
  if( data.loopWifiTimer.lapsed() ) loopWifi();

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
