#define VERSION "1.0.1"

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
#include "Weather.h"
#include "Quote.h"
#include "Display.h"

#include <time.h>
#include <vector>
#include <map>

#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true

/* 
1.0.0 Initial release
1.0.1 Restructured code
      Added precipitation in screen

To do:
  Solve on/off in status
  Add setColor etc in data_t which includes settingsChanged
  Add chosen setting in Telegram keyboard
  Vastly reorganize code
  Allow over the air updates
  Change LIGHT_INC to function in Light.h
*/
 
void setup() {
  // initialize the Serial
  Serial.begin(115200);
  SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED);

  setupLight();
  setupDisplay();
  setupWifi();
  setupTelegram();
}


void loop() {
  static bool wifiConnectReported = false;
  static bool wifiNotConnectReported = false;
  static bool updateScreen = true;

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

  static unsigned long screenChange = millis();

  if(keyLeftCounter ==2) { /* Nothing to do yet */ };
  if(keyTopCounter  ==2) { data.SwitchLight();  };
  if(keyRightCounter==2) { data.screen = scnWeather; updateScreen = true; screenChange = millis(); };
  
  // Increase or decrease light intensity in small timesteps
  const unsigned long lightDimInterval = 100;
  static unsigned long previousLightDim = millis();

  // Tell the light to take the next step
  if (currentTime - previousLightDim >= lightDimInterval ) {
    previousLightDim = currentTime;
    loopLight();
  }  // millis()


  // Switch off the light if the timer elapses
  if ( data.lightOn and (currentTime - data.previousTimer >= TIMES[data.timer] ) ) {
    data.lightOn = false;
    data.UpdateStatus();
  };

  // Switch back to main screen after time has passed
  const unsigned long mainScreenInterval = 10*1000;
  if (currentTime - screenChange >= mainScreenInterval ) {
    screenChange = millis();
    if( data.screen != scnMain ) {
      data.screen = scnMain;
      updateScreen = true;
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
    prev_min = timeinfo->tm_min;
    firstClockUpdate = false;
    if( lightStatus.screen == scnMain ) redrawScreen = true;
  }

  // Periodically get new weather
  static bool firstTimeWeather = true;
  const unsigned long getWeatherInterval = 10*60*1000;
  static unsigned long previousGetWeather = millis();

  if ( firstTimeWeather or (currentTime - previousGetWeather >= getWeatherInterval ) ) {
    Serial.print("Getting the weather...");
    if( getWeather(weather) ) {
      previousGetWeather = currentTime;
      firstTimeWeather = false;
      Serial.println("success");
      if( lightStatus.screen == scnWeather ) redrawScreen = true;
    } else {
      Serial.println("fail");
    }
  };

  if( weather.updated ) {
    if( data.screen == scnWeather ) updateScreen = true;
    weather.updated = false;
  }

  loopDisplay(updateScreen);

  // Handle Telegram messages not already done by the handlers
  loopTelegram();

  // Periodically save settings
  const unsigned long saveSettingsInterval = 10*60*1000;
  static unsigned long previousSaveSettings = millis();

  if (currentTime - previousSaveSettings >= saveSettingsInterval ) {
    previousSaveSettings = currentTime;
    if( data.settingsChanged) data.saveSettings(SPIFFS, SETTINGS_FILE, SETTINGS_TEMP);
  };
  
}
