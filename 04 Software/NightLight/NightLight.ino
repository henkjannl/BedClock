#include <Arduino.h>
#include "esp_system.h"
#include <HTTPClient.h>
#include "FS.h"
#include "SPIFFS.h"

#include <stdlib.h>     

// ======== DEFINES ================
#if CONFIG_FREERTOS_UNICORE
  #define ARDUINO_RUNNING_CORE 0
#else
  #define ARDUINO_RUNNING_CORE 1
#endif

#define FORMAT_SPIFFS_IF_FAILED false

#include "data.h"
#include "keyboard.h"
#include "light.h"
#include "screen.h"
#include "weather.h"
#include "quote.h"

using namespace std;

void initMessage(string msg);

void setup(void) {
  Serial.begin(115200);
  
  delay(100);
  // Initialize SPIFFS
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
      Serial.println("SPIFFS Mount Failed");
      return;
  }
  Serial.println("SPIFFS loaded");  
  config.load();
  
  initMessage("1/7 WiFi");
  while(!data.connected) {
      connectToWiFi();
      delay(250);
    }

  initMessage("2/7 Weather");

  // Retrieve timezone from the weather server
  while(data.weatherAvailable==dqUnavailable) {
    getWeather();
    delay(500);
  }
  Serial.println("Weather setup finished");  

  initMessage("3/7 Sync time");

  while(!data.syncTime) {
      portENTER_CRITICAL(&connectionMux);
      syncTime();
      portEXIT_CRITICAL(&connectionMux);
      delay(300);
    }
  
  initMessage("4/7 Keyboard");

  setupKeyboard();
  Serial.println("Keyboard setup finished");  

  initMessage("5/7 Light");

  setupLight();
  Serial.println("Light setup finished");  

  initMessage("6/7 Screen");
  setupScreen();
  Serial.println("Display setup finished");  

  initMessage("7/7 Quote");
  setupQuote();
  Serial.println("Quote setup finished");  

  initMessage("Setup finished");
} // setup

void loop(void) {
  
  Serial.printf("      Connected: %s Timesync: %s Weather: %s\n", 
     data.connected ? "Y" : "N", 
     data.syncTime ? "Y" : "N", 
     data.weatherAvailable!=dqUnavailable ? "Y" : "N");

  Serial.printf("      Light %d bytes %d updates\n", 
     data.lightHighWaterMark, 
     data.lightAlive);

  Serial.printf("      Screen %d bytes %d updates %d ms redraw time\n", 
     data.screenHighWaterMark,
     data.screenAlive, 
     data.screenRedrawMillis);

  Serial.printf("      Keyboard: %d updates Weather: %d updates %d bytes\n", 
     data.keyboardAlive,
     data.weatherAlive,
     data.weatherHighWaterMark);

  Serial.printf("      Quote: %d updates %d bytes\n", 
     data.quoteAlive,
     data.quoteHighWaterMark);

  Serial.println();

  //portENTER_CRITICAL(&dataAccessMux);
  data.connected = (WiFi.status() == WL_CONNECTED);
  //portEXIT_CRITICAL(&dataAccessMux);  

  //connect to WiFi
  if(!data.connected) connectToWiFi();

  // Sync time
  if(data.connected & !data.syncTime ) syncTime();  

  // Retrieve weather
  if(data.connected & (data.weatherAvailable==dqUnavailable)) getWeather();

  vTaskDelay(10000);
  
} // loop

void initMessage(string msg) {
  static bool init=false;

  if(!init) {
    // Initialize display
    u8g2.initDisplay();
    delay(500);
    u8g2.begin();
    u8g2.enableUTF8Print();
    init=true;
  }
  
  u8g2.clearBuffer();  
  u8g2.setFont(u8g2_font_t0_12b_mf);
  u8g2.drawUTF8(3, 10, "Initialisation:");
  u8g2.setFont(u8g2_font_t0_12_tf); 
  u8g2.drawUTF8(3, 20, msg.c_str());
  u8g2.sendBuffer();
}
