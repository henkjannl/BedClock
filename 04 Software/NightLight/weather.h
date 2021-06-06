#ifndef WEATHER_H

#define WEATHER_H

#include "data.h"
#include <ArduinoJson.h>

TimerHandle_t timerWeather; // New weather report requested if this timer expires

static void timerWeatherCallback( TimerHandle_t xTimer );
void taskWeather(void * parameter );

void setupWeather() {

  xTaskCreatePinnedToCore(
    taskWeather,            // The function containing the task
    "taskWeather",          // Name for mortals
    12000,                  // Stack size 
    NULL,                   // Parameters
    1,                      // Priority, 0=lowest, 3=highest
    NULL,                   // Task handle
    ARDUINO_RUNNING_CORE);  // Core to run the task on

  timerWeather=xTimerCreate( "TimerWeather", 
                10*60*1000,             // New weather report requested every ten minutes
                pdTRUE,                 // Auto repeat timer 
                0,                      // TimerID, unused
                timerWeatherCallback);  // Callback function as timer expires
 
  xTimerStart(timerWeather, 1000);
}; // setupWeather

void getWeather() {
  timerWeatherCallback(timerWeather);
}

static void timerWeatherCallback( TimerHandle_t xTimer ) {
  /*  Retrieving the weather is not directly implemented in the timer
   *  since the code requires more memory than the timer routine has available,
   *  so this results in a stack overflow. Now, retrieving the weather is implemented
   *  in a separate task, taskWeather, and the timer sends a command to this task through
   *  the weatherQueue
   */
   
  data.requestWeather=true;
  Serial.println("Weather requested");
}

void taskWeather(void * parameter ) {

  while(true) {

    if(data.requestWeather) {
      
      Serial.println("Retrieving the weather"); 
      
      if(!data.connected) return;
    
      HTTPClient http;
    
      char buff[250];    
      snprintf(buff, sizeof(buff), "https://api.openweathermap.org/data/2.5/onecall?lat=%.8f&lon=%.8f&exclude=hourly,daily,alerts&appid=%s", 
        data.lat, data.lon, config.openweathermapAPIkey);
      http.begin(buff);
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
        
        portENTER_CRITICAL(&dataAccessMux);
    
        //float lat = doc["lat"]; // 52.2183
        //float lon = doc["lon"]; // 6.8958
        //const char* timezone = doc["timezone"]; // "Europe/Amsterdam"
        //int timezone_offset = doc["timezone_offset"]; // 7200
        
        JsonObject current = doc["current"];
        //long current_dt = current["dt"]; // 1617992165
        //long current_sunrise = current["sunrise"]; // 1617943698
        //long current_sunset = current["sunset"]; // 1617992353
        data.outsideTemp = current["temp"]; // 282.57
        data.outsideTemp-=273.15;
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
        //current_weather_0_description[0]=(char)toupper(current_weather_0_description[0]);
    
        data.precipitation.clear();
        
        for (JsonObject elem : doc["minutely"].as<JsonArray>()) {
          tPrecipitation p;
          p.t = elem["dt"]; // 1617992220, 1617992280, 1617992340, 1617992400, 1617992460, 1617992520, ...
          p.prec = elem["precipitation"]; // 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, ...
          //p.prec= rand() % 3 + 1; // for debugging purposes
          data.precipitation.push_back(p);
        } // elem : minutely

        Serial.printf("%d precipitation points parsed\n", data.precipitation.size());
    
        data.weatherAvailable=dqRefreshed;
        Serial.printf("Outside temperature %.1f C\n", data.outsideTemp);
        data.weatherAlive++;
        data.requestWeather=false;
        portEXIT_CRITICAL(&dataAccessMux);
        
       } // httpCode == HTTP_CODE_OK
      } // httpCode > 0
    } // command received  
    data.weatherHighWaterMark= uxTaskGetStackHighWaterMark(NULL);
    vTaskDelay(1000);
  } // while true
} // timerWeatherCallback

#endif WEATHER_H
