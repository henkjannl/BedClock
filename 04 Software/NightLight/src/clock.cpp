#include "clock.h"
#include <Arduino.h>

#include <HTTPClient.h>
#include "time.h"
#include <ArduinoJson.h>

Clock::Clock() {
  timeZoneKnown=false;
  clockSynched=false;
}

void Clock::requestTimeZone() {

  if(timeZoneKnown) return;

  HTTPClient http;
  
  http.begin(F("http://worldtimeapi.org/api/timezone/Europe/Amsterdam")); 
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

      //const char* abbreviation = doc["abbreviation"]; // "CEST"
      //const char* client_ip = doc["client_ip"]; // "80.100.167.45"
      //const char* datetime = doc["datetime"]; // "2021-04-05T14:28:36.119148+02:00"
      //int day_of_week = doc["day_of_week"]; // 1
      //int day_of_year = doc["day_of_year"]; // 95
      //bool dst = doc["dst"]; // true
      //const char* dst_from = doc["dst_from"]; // "2021-03-28T01:00:00+00:00"
      timeDSToffset = doc["dst_offset"]; // 3600
      //const char* dst_until = doc["dst_until"]; // "2021-10-31T01:00:00+00:00"
      timeZoneOffset = doc["raw_offset"]; // 3600
      //const char* timezone = doc["timezone"]; // "Europe/Amsterdam"
      //long unixtime = doc["unixtime"]; // 1617625716
      //const char* utc_datetime = doc["utc_datetime"]; // "2021-04-05T12:28:36.119148+00:00"
      //const char* utc_offset = doc["utc_offset"]; // "+02:00"
      //int week_number = doc["week_number"]; // 14

      timeZoneKnown=true;
      Serial.println(F("Time zone difference retrieved"));
    }
  }
}

void Clock::syncTime() {
  // Return if timezone not yet known
  if (!timeZoneKnown) return;

  // Return if already synched
  if (clockSynched) return;

  const char* ntpServer = "pool.ntp.org";
  configTime(timeZoneOffset, timeDSToffset, ntpServer);

  // Clear the screen if we receive time
  Serial.println(F("Clock synchronised"));
  clockSynched=true;
}

void Clock::refreshTime() {
  // If too much time has passed, reconnect with the internet to request time
  clockSynched=false;
  timeZoneKnown=false;
}

void Clock::currentTime(char *timeStr) {
  struct tm timeinfo;

  getLocalTime(&timeinfo);
  strftime(timeStr,6,"%H:%M",&timeinfo); // Hour 00-23 : Minute 00-59
}

void Clock::scan() {
  requestTimeZone();
  syncTime();
}

bool Clock::reliable() { 
  return (timeZoneKnown && clockSynched);
}


