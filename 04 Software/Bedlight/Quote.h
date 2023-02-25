#pragma once

#include <HTTPClient.h>
#include <ArduinoJson.h>

String quote() {

  String result;

  HTTPClient http;
  
  http.begin(F("https://api.adviceslip.com/advice"));
  
  int httpCode = http.GET();
  
  if(httpCode > 0) {    
    if(httpCode == HTTP_CODE_OK) {
      StaticJsonDocument<1024> doc;
      
      DeserializationError error = deserializeJson(doc, http.getString());
      
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
      }
      else {
        //int slip_id = doc["slip"]["id"]; // 19
        result = doc["slip"]["advice"].as<String>(); // "If you cannot unscrew the lid of a jar, try placing ...
      }
    }
  }
  return result;
};
