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

#include "data.h"              // Type definitions and global constants
#include "helperfunctions.h"   // Wifi connection and time synchronisation
#include "keyboard.h"          // Monitoring the buttons and sending signals to the screen
#include "light.h"             // LEDs on top of the device
#include "screen.h"            // OLED screen
#include "weather.h"           // Weather server
#include "quote.h"             // Random piece of advice server

using namespace std;

void initMessage(uint8_t step, string msg);

void setup(void) {
  Serial.begin(115200);

  // Initialize display
  delay(500);
  //u8g2.initDisplay();
  //u8g2.setFlipMode(true);
  //delay(500);
  u8g2.setBusClock(50000);
  u8g2.begin();
  
  //u8g2.enableUTF8Print();
  
  initMessage(1, "Config");
  delay(100);
  // Initialize SPIFFS
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
      Serial.println("SPIFFS Mount Failed");
      return;
  }
  Serial.println("SPIFFS loaded");  
  config.load();

  /*
  initMessage(2, "WiFi");
  while(!data.connected) {
    connectToWiFi();
    delay(250);
  }

  initMessage(3, "Time");
  while(!data.syncTime) {
    syncTime();
    delay(3000); // retry not too fast to prevent getting rejected from the API
  }
  */

  /*
  initMessage(4, "Weather");
  while(data.weatherAvailable==dqUnavailable) {
    getWeather();
    delay(3000); // retry not too fast to prevent getting rejected
  }
  Serial.println("Weather setup finished");  
*/
  initMessage(5, "Keyboard");
  setupKeyboard();
  Serial.println("Keyboard setup finished");  

  initMessage(6, "Light");
  setupLight();
  Serial.println("Light setup finished");  

  initMessage(7, "Screen");
  setupScreen();
  Serial.println("Display setup finished");  

/*
  initMessage(8, "Quote");
  getQuote();
  setupQuote();
  Serial.println("Quote setup finished");  
*/
  delay(500);

  initMessage(9, "Ready");

} // setup

void loop(void) {

  Serial.printf("      xPortGetFreeHeapSize = %d Heap valid: %s\n", xPortGetFreeHeapSize(), heap_caps_check_integrity_all(false) ? "Y" : "N");
  
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

  Serial.printf("      Bus clock %d\n", u8g2.getBusClock() );

  Serial.println();
  
  //u8g2.setBusClock(uint32_t clock_speed) { u8g2_GetU8x8(&u8g2)->bus_clock = clock_speed; }
     

/*
  //portENTER_CRITICAL(&dataAccessMux);
  data.connected = (WiFi.status() == WL_CONNECTED);
  //portEXIT_CRITICAL(&dataAccessMux);  

  //connect to WiFi
  if(!data.connected) connectToWiFi();

  // Sync time
  if(data.connected & !data.syncTime ) syncTime();  

  // Retrieve weather
  if(data.connected & (data.weatherAvailable==dqUnavailable)) getWeather();
*/
  delay(10000);
  
} // loop

void initMessage(uint8_t step, string msg) {
  u8g2.clearBuffer();  
  u8g2.setFont(u8g2_font_t0_12b_tf); 
  //u8g2.drawUTF8(3, 12, msg.c_str());
  u8g2.drawStr(3, 12, msg.c_str());
  u8g2.setFont(u8g2_font_t0_12_tf); 
  string progress=string("+++++++++++").substr(0, step)+string("-----------").substr(0, 9-step);
  //u8g2.drawUTF8(3, 30, progress.c_str());
  u8g2.drawStr(3, 30, progress.c_str());
  u8g2.sendBuffer();
  Serial.println(msg.c_str());
  
}
