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

#include <WiFi.h>
#include "time.h"

#include "data.h"
#include "keyboard.h"
#include "light.h"
#include "screen.h"

using namespace std;

void connectToWiFi();
void syncTime();

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
  
  setupKeyboard();
  Serial.println("Keyboard setup finished");  

  setupLight();
  Serial.println("Light setup finished");  

  setupScreen();
  Serial.println("Display setup finished");  

} // setup

void loop(void) {
  
  Serial.printf("Connected: %s Timesync: %s Light %d bytes %d updates Screen %d bytes %d updates %d ms redraw time Keyboard: %d updates\n", 
     data.connected ? "Y" : "N", 
     data.syncTime ? "Y" : "N", 
     data.lightHighWaterMark, 
     data.lightAlive,
     data.screenHighWaterMark,
     data.screenAlive, 
     data.screenRedrawMillis,
     data.keyboardAlive);

  //portENTER_CRITICAL(&dataAccessMux);
  data.connected = (WiFi.status() == WL_CONNECTED);
  //portEXIT_CRITICAL(&dataAccessMux);  

  //connect to WiFi
  if(!data.connected) connectToWiFi();

  // sync time
  if(data.connected & !data.syncTime ) syncTime();  

  vTaskDelay(2500);
  
} // loop

void connectToWiFi() {
  
  /* =========================================================
   *  The code below is way more complex than I'd wish
   *  but somehow I could not make wifiMulti work together 
   *  with FreeRTOS
   *  ======================================================== */
   
  bool connected=false;

  for(auto accessPoint : config.AccessPoints) Serial.println(accessPoint.ssid);

  int n = WiFi.scanNetworks();
  for (int i = 0; i < n; ++i) {
    for(auto accessPoint : config.AccessPoints) {
      if( WiFi.SSID(i) == String(accessPoint.ssid) ) {
          Serial.printf("Connecting to %s\n", accessPoint.ssid);  
          WiFi.begin(accessPoint.ssid, accessPoint.password);
          for(int j=0; j<35; j++) {
            if (WiFi.status() != WL_CONNECTED)
              {
                Serial.print(".");
                vTaskDelay(500);
              }
            else {
              Serial.printf("\nWiFi connected to %s.\n", WiFi.SSID());
              
              portENTER_CRITICAL(&dataAccessMux);
              data.timezone=accessPoint.timezone;
              portEXIT_CRITICAL(&dataAccessMux);  
              connected = true;

              break;
            } // not yet connected
            if(connected) break; // break j loop
        } // for j
      } // if ssid==ssid
      if(connected) break; // break accessPoint loop
    } // for accessPoint
    if(connected) break; // break i loop
  } // for i

  portENTER_CRITICAL(&dataAccessMux);
  data.connected = (WiFi.status() == WL_CONNECTED);
  portEXIT_CRITICAL(&dataAccessMux);  
} // connectToWifi


void syncTime() {
  string url;
  
  HTTPClient http;

  url=string("http://worldtimeapi.org/api/timezone/") + data.timezone;
  http.begin(url.c_str()); 
  
  int httpCode = http.GET();
  
  if(httpCode > 0) {    
    if(httpCode == HTTP_CODE_OK) {
      StaticJsonDocument<1024> doc;

      DeserializationError error = deserializeJson(doc, http.getString());

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      portENTER_CRITICAL(&dataAccessMux);
        //const char* abbreviation = doc["abbreviation"]; // "CEST"
        //const char* client_ip = doc["client_ip"]; // "80.100.167.45"
        //const char* datetime = doc["datetime"]; // "2021-04-05T14:28:36.119148+02:00"
        //int day_of_week = doc["day_of_week"]; // 1
        //int day_of_year = doc["day_of_year"]; // 95
        //bool dst = doc["dst"]; // true
        //const char* dst_from = doc["dst_from"]; // "2021-03-28T01:00:00+00:00"
        data.timeDSToffset = doc["dst_offset"]; // 3600
        //const char* dst_until = doc["dst_until"]; // "2021-10-31T01:00:00+00:00"
        data.timeZoneOffset = doc["raw_offset"]; // 3600
        //const char* timezone = doc["timezone"]; // "Europe/Amsterdam"
        //long unixtime = doc["unixtime"]; // 1617625716
        //const char* utc_datetime = doc["utc_datetime"]; // "2021-04-05T12:28:36.119148+00:00"
        //const char* utc_offset = doc["utc_offset"]; // "+02:00"
        //int week_number = doc["week_number"]; // 14
      portEXIT_CRITICAL(&dataAccessMux);  

      configTime(data.timeZoneOffset, data.timeDSToffset, "pool.ntp.org");
    
      portENTER_CRITICAL(&dataAccessMux);
        data.syncTime = true;
      portEXIT_CRITICAL(&dataAccessMux);  
    }
  }
} // syncTime
