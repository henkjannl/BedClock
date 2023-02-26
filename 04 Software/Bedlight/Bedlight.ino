#define VERSION "1.2.0"

/* 
1.0.0 Initial release
1.1.0 Restructured code
      Added precipitation in screen on press of right button
1.2.0 Added advice on press of left button
      Most optimal font for advice chosen

To do:
  Add chosen setting in Telegram keyboard
  Allow over the air updates
  Change LIGHT_INC to function in Light.h
*/

/*
MyCredentials.h is in gitignore and should have the following content:

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
#include "Data.h"
#include "Light.h"
#include "Telegram.h"
#include "Display.h"
#include "Weather.h"
#include "Advice.h"

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
  
  //listDir(SPIFFS, "/" , 8);  
}


void loop() {
  static bool wifiConnectReported = false;
  static bool wifiNotConnectReported = false;
  unsigned long currentTime;
  static unsigned long screenChange = millis();

  // Read the clock for this round
  currentTime = millis();

  // Check if WiFi is still alive
  if (wifiMulti.run( CONNECT_TIMEOUT_MS ) == WL_CONNECTED) {  /*if the connection lost it will connect to next network*/
    if( !wifiConnectReported ) {
      Serial.printf( "WiFi connected to SSID %s signl strength %ddB\n", WiFi.SSID().c_str(), WiFi.RSSI() );
      wifiConnectReported=true;
      wifiNotConnectReported=false;
    };
  }
  else {
    if( !wifiNotConnectReported ) {
      Serial.println("WiFi not connected!");  /*if all conditions fail print this*/
      wifiNotConnectReported=true;
      wifiConnectReported=false;
    }
  }

  // Check if button is pressed
  static volatile uint32_t keyLeftCounter  = 0;
  static volatile uint32_t keyRightCounter = 0;
  static volatile uint32_t keyTopCounter   = 0;

  if (touchRead(PIN_KEY_LEFT )<KEY_TRESHOLD) keyLeftCounter++;  else keyLeftCounter =0; 
  if (touchRead(PIN_KEY_TOP  )<KEY_TRESHOLD) keyTopCounter++;   else keyTopCounter  =0; 
  if (touchRead(PIN_KEY_RIGHT)<KEY_TRESHOLD) keyRightCounter++; else keyRightCounter=0; 

  if(keyLeftCounter ==2) { 
    data.screen = scnAdvice; 
    data.updateScreen = true; 
    screenChange = currentTime;     
  };

  if(keyTopCounter  ==2) { data.switchLight();  };

  if(keyRightCounter==2) { 
    data.screen = scnWeather; 
    data.updateScreen = true; 
    screenChange = currentTime; 
  };

  // Increase or decrease light intensity in tiny timesteps
  const unsigned long lightDimInterval = 100;
  static unsigned long previousLightDim = currentTime;

  // Tell the light to take the next step
  if (currentTime - previousLightDim >= lightDimInterval ) {
    previousLightDim = currentTime;
    loopLight();
  }  

  // Switch off the light if the timer elapses
  if ( data.lightOn and (currentTime - data.previousTimer >= TIMES[data.timer] ) ) data.setLightOff();

  // Retrieve weather every now and then so it is available upon request
  const unsigned long weatherInterval = 12*60*1000;
  static unsigned long lastTimeWeatherWasFetched = currentTime;
  if ( currentTime - lastTimeWeatherWasFetched >= weatherInterval ) {
    data.requestNewWeather = true;
    lastTimeWeatherWasFetched = currentTime;
  }

  if( data.requestNewWeather ) getWeather();

  // Switch back to main screen after time has passed
  const unsigned long mainScreenInterval = 10*1000;
  if (currentTime - screenChange >= mainScreenInterval ) {
    screenChange = currentTime;
    if( data.screen != scnMain ) {
      resetScreen(); // Reset display sometimes, cause it sometimes stops working
      data.screen = scnMain;
      data.updateScreen = true;
    };
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

  // Only call loopDisplay if screen update is requested
  if( data.updateScreen ) { 
    loopDisplay();
    
    // After display is refreshed, get new advice for next time an advice is requested
    if( data.screen == scnAdvice ) getAdvice();
  }

  // Handle Telegram messages not already done by the handlers
  loopTelegram();

  // Periodically save settings
  const unsigned long saveSettingsInterval = 3*60*1000;
  static unsigned long previousSaveSettings = millis();

  if (currentTime - previousSaveSettings >= saveSettingsInterval ) {
    previousSaveSettings = currentTime;
    if( data.settingsChanged) { 
      data.saveSettings(SPIFFS, SETTINGS_FILE, SETTINGS_TEMP);
      listDir(SPIFFS, "/", 6);
    }
  };

  // Get first advice. Subsequent advices will be retrieved when an advice is displayed
  if( data.newAdviceRequested ) getAdvice();
  
}
