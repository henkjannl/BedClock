#define VERSION "1.2.0"

/* 
1.0.0 First working version
1.1.0 Restructured code
      Added precipitation in screen on press of right button
      Saving /loading settings finally works
1.2.0 Added advice on press of left button
      Most optimal font for advice chosen
      Timers in separate class

To do:
  Reintroduce weather icons
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

#include "MyCredentials.h" // All modules have access to data in MyCredentials
#include "a_Data.h"          // This file contains all types and the struct 'data' which acts as a central databus
#include "b_Light.h"
#include "c_WiFi.h"
#include "d_Telegram.h"
#include "e_Display.h"
#include "f_Weather.h"
#include "g_Advice.h"

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
}


void loop() {

  loopWifi();

  // Check if button is pressed
  static volatile uint32_t keyLeftCounter  = 0;
  static volatile uint32_t keyRightCounter = 0;
  static volatile uint32_t keyTopCounter   = 0;

  if (touchRead(PIN_KEY_LEFT )<KEY_TRESHOLD) keyLeftCounter++;  else keyLeftCounter =0; 
  if (touchRead(PIN_KEY_TOP  )<KEY_TRESHOLD) keyTopCounter++;   else keyTopCounter  =0; 
  if (touchRead(PIN_KEY_RIGHT)<KEY_TRESHOLD) keyRightCounter++; else keyRightCounter=0; 

  if( keyLeftCounter == 2 ) { 
    data.screen = scnAdvice; 
    data.screenBacktoMainTimer.reset();
    data.updateScreen = true; 
  };

  if( keyTopCounter == 2 ) { 
    data.switchLight();    
    Serial.printf( "Light switched %s\n", data.lightOn ? "on" : "off" );
  };

  if( keyRightCounter == 2 ) { 
    data.screen = scnWeather; 
    data.screenBacktoMainTimer.reset();
    data.updateScreen = true; 
  };

  // Tell the light to take the next step
  if( data.lightStepTimer.lapsed() ) {
    loopLight();
    if( !data.R.ready() ) Serial.printf("Red ready: %s value %d target %.3f\n", data.R.ready() ? "Y" : "N", data.R.getValue(), data.R.getTarget() );
  }  

  // Switch the light off if the timer elapses
  if ( data.lightOn and (data.switchLightOffTimer.lapsed() ) ) data.setLightOff();

  // Retrieve weather every now and then so it is available upon request
  if ( data.refreshWeatherTimer.lapsed() ) data.requestNewWeather = true;
  if( data.requestNewWeather ) getWeather();

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
  static bool firstClockUpdate = true;
  
  time( &rawtime );
  timeinfo = localtime( &rawtime );
  if( firstClockUpdate or ( ( timeinfo->tm_year > 80 ) and ( timeinfo->tm_min != prev_min ) ) ) {
    firstClockUpdate = false;
    prev_min = timeinfo->tm_min;
    snprintf(data.displayClock, sizeof( data.displayClock ), "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min );
    if( data.screen == scnMain ) data.updateScreen = true;
  }

  // Repeat getting advice if previous advice was too long to be displayed
  if( data.newAdviceRequested ) getAdvice();

  // Only call loopDisplay if screen update is requested
  if( data.updateScreen ) { 
    loopDisplay();
    
    // After display is refreshed, get new advice for next time an advice is requested
    if( data.screen == scnAdvice ) getAdvice();
  }

  // Handle Telegram messages not already done by the handlers
  loopTelegram();

  // Periodically save settings
  if (data.saveSettingsTimer.lapsed() ) {
    if( data.settingsChanged) { 
      data.saveSettings(SPIFFS, SETTINGS_FILE, SETTINGS_TEMP);
      listDir(SPIFFS, "/", 6);
    }
  }  
}
