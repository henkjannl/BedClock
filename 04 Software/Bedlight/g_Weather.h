#pragma once

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <list>

void getWeather() {
      
  Serial.println("Retrieving the weather"); 
  
  HTTPClient http;

  char request[250];    
  snprintf(request, sizeof(request), "https://api.openweathermap.org/data/2.5/onecall?lat=%.8f&lon=%.8f&exclude=hourly,daily,alerts&units=metric&appid=%s", 
    latitude, longitude, openweathermapAPIkey);
  http.begin(request);
  int httpCode = http.GET();
  
  if(httpCode > 0) {    
    if(httpCode == HTTP_CODE_OK) {
      
      DynamicJsonDocument doc(4096);
      
      DeserializationError error = deserializeJson(doc, http.getString());
      
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      
      //float lat = doc["lat"]; // 52.2183
      //float lon = doc["lon"]; // 6.8958
      //const char* timezone = doc["timezone"]; // "Europe/Amsterdam"
      //int timezone_offset = doc["timezone_offset"]; // 7200
      
      JsonObject current = doc["current"];
      //long current_dt = current["dt"]; // 1617992165
      data.sunrise = current["sunrise"]; // 1617943698
      data.sunset  = current["sunset"]; // 1617992353
      data.outsideTemp = current["temp"]; // 282.57
      //data.outsideTemp-=273.15;
      //int current_feels_like = current["feels_like"]; // 281
      //int current_pressure = current["pressure"]; // 1008
      //int current_humidity = current["humidity"]; // 60
      //float current_dew_point = current["dew_point"]; // 275.2
      //int current_uvi = current["uvi"]; // 0
      //int current_clouds = current["clouds"]; // 95
      //int current_visibility = current["visibility"]; // 10000
      //float current_wind_speed = current["wind_speed"]; // 2.95
      //int current_wind_deg = current["wind_deg"]; // 294
      //float current_wind_gust = current["wind_gust"]; // 5.76
      
      JsonObject current_weather_0 = current["weather"][0];
      //int current_weather_0_id = current_weather_0["id"]; // 804
      //const char* current_weather_0_main = current_weather_0["main"]; // "Clouds"
      const char* current_weather_0_description = current_weather_0["description"]; // "overcast clouds"
      const char* current_weather_0_icon = current_weather_0["icon"]; // "04d"
      strcpy(data.weatherIcon, current_weather_0_icon);    
      Serial.printf("Weather icon: %s\n", data.weatherIcon);
      //current_weather_0_description[0]=(char)toupper(current_weather_0_description[0]);
  
      data.precipitation.clear();
      data.precipitationExpected = false;
      
      for (JsonObject elem : doc["minutely"].as<JsonArray>()) {
        precipitation_t p;
        p.t = elem["dt"]; // 1617992220, 1617992280, 1617992340, 1617992400, 1617992460, 1617992520, ...
        p.prec = elem["precipitation"]; // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ...

        // p.prec= rand() % 3 + 1; // for debugging purposes

        data.precipitation.push_back(p);
        if( p.prec>0 ) data.precipitationExpected = true;
      } // elem : minutely
  
      Serial.printf("%d precipitation points parsed\n", data.precipitation.size());  
      Serial.printf("Outside temperature %.1f C\n", data.outsideTemp);
    
   } else return; // httpCode == HTTP_CODE_OK
  } else return; // httpCode > 0

  // Everything went well
  Serial.println("Weather request went well");
  snprintf( data.displayTemperature, sizeof( data.displayTemperature ), "%0d%sC", (int) data.outsideTemp, DEGREE_SYMBOL );
  data.weatherUpdated = true;
  data.requestNewWeather = false;
} // getWeather