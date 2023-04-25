#pragma once

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <list>
#include <ctime>

using namespace std;


void getWeather() {

  Serial.println("Retrieving the weather"); 
  data.weatherRetrievalCounter++;
  
  HTTPClient http;

  char request[250];    
  snprintf(request, sizeof(request), "https://api.openweathermap.org/data/2.5/onecall?lat=%.8f&lon=%.8f&exclude=hourly,alerts&units=metric&appid=%s", 
    latitude, longitude, openweathermapAPIkey);
  /*
  snprintf(request, sizeof(request), "https://api.openweathermap.org/data/3.0/onecall?lat=%.8f&lon=%.8f&exclude=hourly,daily,alerts&units=metric&appid=%s", 
    latitude, longitude, openweathermapAPIkey);  
  */

  Serial.println( request ); 
    
  http.begin(request);
  int httpCode = http.GET();
  
  if(httpCode > 0) {    
    if(httpCode == HTTP_CODE_OK) {
      
      DynamicJsonDocument doc(16384);
      
      DeserializationError error = deserializeJson(doc, http.getString());
      
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      Serial.println("Still retrieving the weather");
      
      //====================
      //float lat = doc["lat"]; // 52.2532
      //float lon = doc["lon"]; // 6.7855
      //const char* timezone = doc["timezone"]; // "Europe/Amsterdam"
      //int timezone_offset = doc["timezone_offset"]; // 7200
      
      JsonObject current = doc["current"];
      //long current_dt = current["dt"]; // 1680108941
      data.sunrise = current["sunrise"]; // 1617943698
      data.sunset  = current["sunset"]; // 1617992353
      data.outsideTemp = current["temp"]; // 282.57
      data.outsideFeelsLike = current["feels_like"]; // 282.57
      //int current_pressure = current["pressure"]; // 1008
      data.humidity = current["humidity"]; // 60
      //float current_dew_point = current["dew_point"]; // 9.84
      //float current_uvi = current["uvi"]; // 0.1
      //int current_clouds = current["clouds"]; // 100
      //int current_visibility = current["visibility"]; // 10000
      //float current_wind_speed = current["wind_speed"]; // 1.79
      //int current_wind_deg = current["wind_deg"]; // 167
      //float current_wind_gust = current["wind_gust"]; // 3.13
      
      JsonObject current_weather_0 = current["weather"][0];
      //const char* current_weather_0_icon = current_weather_0["icon"]; // "04d"
      strcpy(data.weatherIcon, current_weather_0["icon"]);
          
      Serial.printf("Weather icon: %s\n", data.weatherIcon);
      //const char* current_weather_0_main = current_weather_0["main"]; // "Clouds"
      //const char* current_weather_0_description = current_weather_0["description"]; // "overcast clouds"
      //const char* current_weather_0_icon = current_weather_0["icon"]; // "04d"
      
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
      
      JsonArray daily = doc["daily"];

      JsonObject daily_0 = daily[0];
      /*
      long daily_0_dt = daily_0["dt"]; // 1680087600
      long daily_0_sunrise = daily_0["sunrise"]; // 1680066921
      long daily_0_sunset = daily_0["sunset"]; // 1680112794
      long daily_0_moonrise = daily_0["moonrise"]; // 1680080880
      long daily_0_moonset = daily_0["moonset"]; // 1680057300
      float daily_0_moon_phase = daily_0["moon_phase"]; // 0.25
      */
      
      JsonObject daily_0_temp = daily_0["temp"];
      //float daily_0_temp_day = daily_0_temp["day"]; // 10.02
      //float daily_0_temp_min = daily_0_temp["min"]; // 4.65
      data.maxDayTemp = daily_0_temp["max"]; // 13.13
      //float daily_0_temp_night = daily_0_temp["night"]; // 12.19
      //float daily_0_temp_eve = daily_0_temp["eve"]; // 12.65
      //float daily_0_temp_morn = daily_0_temp["morn"]; // 5.99

      /*
      JsonObject daily_0_feels_like = daily_0["feels_like"];
      float daily_0_feels_like_day = daily_0_feels_like["day"]; // 9.19
      float daily_0_feels_like_night = daily_0_feels_like["night"]; // 11.74
      float daily_0_feels_like_eve = daily_0_feels_like["eve"]; // 12.14
      float daily_0_feels_like_morn = daily_0_feels_like["morn"]; // 2.86
      
      int daily_0_pressure = daily_0["pressure"]; // 1014
      int daily_0_humidity = daily_0["humidity"]; // 81
      float daily_0_dew_point = daily_0["dew_point"]; // 6.76
      float daily_0_wind_speed = daily_0["wind_speed"]; // 6.4
      int daily_0_wind_deg = daily_0["wind_deg"]; // 206
      float daily_0_wind_gust = daily_0["wind_gust"]; // 14.21
      
      JsonObject daily_0_weather_0 = daily_0["weather"][0];
      int daily_0_weather_0_id = daily_0_weather_0["id"]; // 804
      const char* daily_0_weather_0_main = daily_0_weather_0["main"]; // "Clouds"
      const char* daily_0_weather_0_description = daily_0_weather_0["description"]; // "overcast clouds"
      const char* daily_0_weather_0_icon = daily_0_weather_0["icon"]; // "04d"
      
      int daily_0_clouds = daily_0["clouds"]; // 100
      float daily_0_pop = daily_0["pop"]; // 0.41
      float daily_0_uvi = daily_0["uvi"]; // 2.63
      
      JsonObject daily_1 = daily[1];
      long daily_1_dt = daily_1["dt"]; // 1680174000
      long daily_1_sunrise = daily_1["sunrise"]; // 1680153180
      long daily_1_sunset = daily_1["sunset"]; // 1680199298
      long daily_1_moonrise = daily_1["moonrise"]; // 1680171060
      long daily_1_moonset = daily_1["moonset"]; // 1680146280
      float daily_1_moon_phase = daily_1["moon_phase"]; // 0.29
      
      JsonObject daily_1_temp = daily_1["temp"];
      float daily_1_temp_day = daily_1_temp["day"]; // 12.06
      float daily_1_temp_min = daily_1_temp["min"]; // 8.85
      float daily_1_temp_max = daily_1_temp["max"]; // 13.3
      float daily_1_temp_night = daily_1_temp["night"]; // 8.85
      float daily_1_temp_eve = daily_1_temp["eve"]; // 11.9
      float daily_1_temp_morn = daily_1_temp["morn"]; // 10.64
      
      JsonObject daily_1_feels_like = daily_1["feels_like"];
      float daily_1_feels_like_day = daily_1_feels_like["day"]; // 11.41
      float daily_1_feels_like_night = daily_1_feels_like["night"]; // 6.4
      float daily_1_feels_like_eve = daily_1_feels_like["eve"]; // 11.18
      float daily_1_feels_like_morn = daily_1_feels_like["morn"]; // 10.21
      
      int daily_1_pressure = daily_1["pressure"]; // 1006
      int daily_1_humidity = daily_1["humidity"]; // 80
      float daily_1_dew_point = daily_1["dew_point"]; // 8.68
      float daily_1_wind_speed = daily_1["wind_speed"]; // 10.38
      int daily_1_wind_deg = daily_1["wind_deg"]; // 246
      float daily_1_wind_gust = daily_1["wind_gust"]; // 17.61
      
      JsonObject daily_1_weather_0 = daily_1["weather"][0];
      int daily_1_weather_0_id = daily_1_weather_0["id"]; // 501
      const char* daily_1_weather_0_main = daily_1_weather_0["main"]; // "Rain"
      const char* daily_1_weather_0_description = daily_1_weather_0["description"]; // "moderate rain"
      const char* daily_1_weather_0_icon = daily_1_weather_0["icon"]; // "10d"
      
      int daily_1_clouds = daily_1["clouds"]; // 97
      int daily_1_pop = daily_1["pop"]; // 1
      float daily_1_rain = daily_1["rain"]; // 5.78
      float daily_1_uvi = daily_1["uvi"]; // 2.11
      
      JsonObject daily_2 = daily[2];
      long daily_2_dt = daily_2["dt"]; // 1680260400
      long daily_2_sunrise = daily_2["sunrise"]; // 1680239439
      long daily_2_sunset = daily_2["sunset"]; // 1680285801
      long daily_2_moonrise = daily_2["moonrise"]; // 1680261720
      long daily_2_moonset = daily_2["moonset"]; // 1680234540
      float daily_2_moon_phase = daily_2["moon_phase"]; // 0.32
      
      JsonObject daily_2_temp = daily_2["temp"];
      float daily_2_temp_day = daily_2_temp["day"]; // 12.36
      float daily_2_temp_min = daily_2_temp["min"]; // 8.63
      float daily_2_temp_max = daily_2_temp["max"]; // 12.36
      float daily_2_temp_night = daily_2_temp["night"]; // 8.81
      float daily_2_temp_eve = daily_2_temp["eve"]; // 10.64
      float daily_2_temp_morn = daily_2_temp["morn"]; // 9.55
      
      JsonObject daily_2_feels_like = daily_2["feels_like"];
      float daily_2_feels_like_day = daily_2_feels_like["day"]; // 11.66
      float daily_2_feels_like_night = daily_2_feels_like["night"]; // 5.2
      float daily_2_feels_like_eve = daily_2_feels_like["eve"]; // 9.87
      float daily_2_feels_like_morn = daily_2_feels_like["morn"]; // 6.87
      
      int daily_2_pressure = daily_2["pressure"]; // 997
      int daily_2_humidity = daily_2["humidity"]; // 77
      float daily_2_dew_point = daily_2["dew_point"]; // 8.36
      float daily_2_wind_speed = daily_2["wind_speed"]; // 8.75
      int daily_2_wind_deg = daily_2["wind_deg"]; // 236
      float daily_2_wind_gust = daily_2["wind_gust"]; // 13.53
      
      JsonObject daily_2_weather_0 = daily_2["weather"][0];
      int daily_2_weather_0_id = daily_2_weather_0["id"]; // 501
      const char* daily_2_weather_0_main = daily_2_weather_0["main"]; // "Rain"
      const char* daily_2_weather_0_description = daily_2_weather_0["description"]; // "moderate rain"
      const char* daily_2_weather_0_icon = daily_2_weather_0["icon"]; // "10d"
      
      int daily_2_clouds = daily_2["clouds"]; // 100
      int daily_2_pop = daily_2["pop"]; // 1
      float daily_2_rain = daily_2["rain"]; // 10.76
      float daily_2_uvi = daily_2["uvi"]; // 0.29
      
      JsonObject daily_3 = daily[3];
      long daily_3_dt = daily_3["dt"]; // 1680346800
      long daily_3_sunrise = daily_3["sunrise"]; // 1680325699
      long daily_3_sunset = daily_3["sunset"]; // 1680372304
      long daily_3_moonrise = daily_3["moonrise"]; // 1680352500
      long daily_3_moonset = daily_3["moonset"]; // 1680322260
      float daily_3_moon_phase = daily_3["moon_phase"]; // 0.35
      
      JsonObject daily_3_temp = daily_3["temp"];
      float daily_3_temp_day = daily_3_temp["day"]; // 9.35
      float daily_3_temp_min = daily_3_temp["min"]; // 4.25
      float daily_3_temp_max = daily_3_temp["max"]; // 9.35
      float daily_3_temp_night = daily_3_temp["night"]; // 4.25
      float daily_3_temp_eve = daily_3_temp["eve"]; // 5.66
      float daily_3_temp_morn = daily_3_temp["morn"]; // 8.87
      
      JsonObject daily_3_feels_like = daily_3["feels_like"];
      float daily_3_feels_like_day = daily_3_feels_like["day"]; // 8.49
      float daily_3_feels_like_night = daily_3_feels_like["night"]; // -0.93
      float daily_3_feels_like_eve = daily_3_feels_like["eve"]; // 0.78
      float daily_3_feels_like_morn = daily_3_feels_like["morn"]; // 5.9
      
      int daily_3_pressure = daily_3["pressure"]; // 998
      int daily_3_humidity = daily_3["humidity"]; // 94
      float daily_3_dew_point = daily_3["dew_point"]; // 8.32
      float daily_3_wind_speed = daily_3["wind_speed"]; // 8.83
      int daily_3_wind_deg = daily_3["wind_deg"]; // 61
      float daily_3_wind_gust = daily_3["wind_gust"]; // 14.88
      
      JsonObject daily_3_weather_0 = daily_3["weather"][0];
      int daily_3_weather_0_id = daily_3_weather_0["id"]; // 501
      const char* daily_3_weather_0_main = daily_3_weather_0["main"]; // "Rain"
      const char* daily_3_weather_0_description = daily_3_weather_0["description"]; // "moderate rain"
      const char* daily_3_weather_0_icon = daily_3_weather_0["icon"]; // "10d"
      
      int daily_3_clouds = daily_3["clouds"]; // 100
      int daily_3_pop = daily_3["pop"]; // 1
      float daily_3_rain = daily_3["rain"]; // 13.59
      float daily_3_uvi = daily_3["uvi"]; // 0.72
      
      JsonObject daily_4 = daily[4];
      long daily_4_dt = daily_4["dt"]; // 1680433200
      long daily_4_sunrise = daily_4["sunrise"]; // 1680411959
      long daily_4_sunset = daily_4["sunset"]; // 1680458807
      long daily_4_moonrise = daily_4["moonrise"]; // 1680443340
      long daily_4_moonset = daily_4["moonset"]; // 1680409620
      float daily_4_moon_phase = daily_4["moon_phase"]; // 0.38
      
      JsonObject daily_4_temp = daily_4["temp"];
      float daily_4_temp_day = daily_4_temp["day"]; // 7.74
      float daily_4_temp_min = daily_4_temp["min"]; // 0.47
      float daily_4_temp_max = daily_4_temp["max"]; // 8.62
      float daily_4_temp_night = daily_4_temp["night"]; // 3.09
      float daily_4_temp_eve = daily_4_temp["eve"]; // 5.29
      float daily_4_temp_morn = daily_4_temp["morn"]; // 0.47
      
      JsonObject daily_4_feels_like = daily_4["feels_like"];
      float daily_4_feels_like_day = daily_4_feels_like["day"]; // 4.39
      float daily_4_feels_like_night = daily_4_feels_like["night"]; // 0.11
      float daily_4_feels_like_eve = daily_4_feels_like["eve"]; // 2.62
      float daily_4_feels_like_morn = daily_4_feels_like["morn"]; // -5.25
      
      int daily_4_pressure = daily_4["pressure"]; // 1026
      int daily_4_humidity = daily_4["humidity"]; // 49
      float daily_4_dew_point = daily_4["dew_point"]; // -2.42
      float daily_4_wind_speed = daily_4["wind_speed"]; // 7.87
      int daily_4_wind_deg = daily_4["wind_deg"]; // 57
      float daily_4_wind_gust = daily_4["wind_gust"]; // 13.41
      
      JsonObject daily_4_weather_0 = daily_4["weather"][0];
      int daily_4_weather_0_id = daily_4_weather_0["id"]; // 800
      const char* daily_4_weather_0_main = daily_4_weather_0["main"]; // "Clear"
      const char* daily_4_weather_0_description = daily_4_weather_0["description"]; // "clear sky"
      const char* daily_4_weather_0_icon = daily_4_weather_0["icon"]; // "01d"
      
      int daily_4_clouds = daily_4["clouds"]; // 7
      float daily_4_pop = daily_4["pop"]; // 0.09
      float daily_4_uvi = daily_4["uvi"]; // 2.95
      
      JsonObject daily_5 = daily[5];
      long daily_5_dt = daily_5["dt"]; // 1680519600
      long daily_5_sunrise = daily_5["sunrise"]; // 1680498220
      long daily_5_sunset = daily_5["sunset"]; // 1680545310
      long daily_5_moonrise = daily_5["moonrise"]; // 1680534180
      long daily_5_moonset = daily_5["moonset"]; // 1680496800
      float daily_5_moon_phase = daily_5["moon_phase"]; // 0.41
      
      JsonObject daily_5_temp = daily_5["temp"];
      float daily_5_temp_day = daily_5_temp["day"]; // 8.36
      float daily_5_temp_min = daily_5_temp["min"]; // 0.39
      float daily_5_temp_max = daily_5_temp["max"]; // 8.85
      float daily_5_temp_night = daily_5_temp["night"]; // 3.27
      float daily_5_temp_eve = daily_5_temp["eve"]; // 5.97
      float daily_5_temp_morn = daily_5_temp["morn"]; // 0.92
      
      JsonObject daily_5_feels_like = daily_5["feels_like"];
      float daily_5_feels_like_day = daily_5_feels_like["day"]; // 6.95
      float daily_5_feels_like_night = daily_5_feels_like["night"]; // 0.67
      float daily_5_feels_like_eve = daily_5_feels_like["eve"]; // 3.98
      float daily_5_feels_like_morn = daily_5_feels_like["morn"]; // -1.6
      
      int daily_5_pressure = daily_5["pressure"]; // 1030
      int daily_5_humidity = daily_5["humidity"]; // 44
      float daily_5_dew_point = daily_5["dew_point"]; // -3.07
      float daily_5_wind_speed = daily_5["wind_speed"]; // 2.81
      int daily_5_wind_deg = daily_5["wind_deg"]; // 66
      float daily_5_wind_gust = daily_5["wind_gust"]; // 5.21
      
      JsonObject daily_5_weather_0 = daily_5["weather"][0];
      int daily_5_weather_0_id = daily_5_weather_0["id"]; // 802
      const char* daily_5_weather_0_main = daily_5_weather_0["main"]; // "Clouds"
      const char* daily_5_weather_0_description = daily_5_weather_0["description"]; // "scattered clouds"
      const char* daily_5_weather_0_icon = daily_5_weather_0["icon"]; // "03d"
      
      int daily_5_clouds = daily_5["clouds"]; // 46
      int daily_5_pop = daily_5["pop"]; // 0
      int daily_5_uvi = daily_5["uvi"]; // 3
      
      JsonObject daily_6 = daily[6];
      long daily_6_dt = daily_6["dt"]; // 1680606000
      long daily_6_sunrise = daily_6["sunrise"]; // 1680584480
      long daily_6_sunset = daily_6["sunset"]; // 1680631813
      long daily_6_moonrise = daily_6["moonrise"]; // 1680624960
      long daily_6_moonset = daily_6["moonset"]; // 1680583920
      float daily_6_moon_phase = daily_6["moon_phase"]; // 0.44
      
      JsonObject daily_6_temp = daily_6["temp"];
      float daily_6_temp_day = daily_6_temp["day"]; // 8.92
      float daily_6_temp_min = daily_6_temp["min"]; // 0.36
      float daily_6_temp_max = daily_6_temp["max"]; // 9.61
      float daily_6_temp_night = daily_6_temp["night"]; // 5.88
      float daily_6_temp_eve = daily_6_temp["eve"]; // 6.98
      float daily_6_temp_morn = daily_6_temp["morn"]; // 1.15
      
      JsonObject daily_6_feels_like = daily_6["feels_like"];
      float daily_6_feels_like_day = daily_6_feels_like["day"]; // 8.92
      float daily_6_feels_like_night = daily_6_feels_like["night"]; // 5.88
      float daily_6_feels_like_eve = daily_6_feels_like["eve"]; // 6.98
      float daily_6_feels_like_morn = daily_6_feels_like["morn"]; // 1.15
      
      int daily_6_pressure = daily_6["pressure"]; // 1024
      int daily_6_humidity = daily_6["humidity"]; // 46
      int daily_6_dew_point = daily_6["dew_point"]; // -2
      float daily_6_wind_speed = daily_6["wind_speed"]; // 1.92
      int daily_6_wind_deg = daily_6["wind_deg"]; // 40
      float daily_6_wind_gust = daily_6["wind_gust"]; // 1.92
      
      JsonObject daily_6_weather_0 = daily_6["weather"][0];
      int daily_6_weather_0_id = daily_6_weather_0["id"]; // 800
      const char* daily_6_weather_0_main = daily_6_weather_0["main"]; // "Clear"
      const char* daily_6_weather_0_description = daily_6_weather_0["description"]; // "clear sky"
      const char* daily_6_weather_0_icon = daily_6_weather_0["icon"]; // "01d"
      
      int daily_6_clouds = daily_6["clouds"]; // 6
      int daily_6_pop = daily_6["pop"]; // 0
      int daily_6_uvi = daily_6["uvi"]; // 3
      
      JsonObject daily_7 = daily[7];
      long daily_7_dt = daily_7["dt"]; // 1680692400
      long daily_7_sunrise = daily_7["sunrise"]; // 1680670742
      long daily_7_sunset = daily_7["sunset"]; // 1680718316
      long daily_7_moonrise = daily_7["moonrise"]; // 1680715860
      long daily_7_moonset = daily_7["moonset"]; // 1680670920
      float daily_7_moon_phase = daily_7["moon_phase"]; // 0.47
      
      JsonObject daily_7_temp = daily_7["temp"];
      float daily_7_temp_day = daily_7_temp["day"]; // 5.83
      float daily_7_temp_min = daily_7_temp["min"]; // 4.49
      float daily_7_temp_max = daily_7_temp["max"]; // 8.23
      float daily_7_temp_night = daily_7_temp["night"]; // 7.59
      float daily_7_temp_eve = daily_7_temp["eve"]; // 8.23
      float daily_7_temp_morn = daily_7_temp["morn"]; // 4.49
      
      JsonObject daily_7_feels_like = daily_7["feels_like"];
      float daily_7_feels_like_day = daily_7_feels_like["day"]; // 2.57
      float daily_7_feels_like_night = daily_7_feels_like["night"]; // 5.89
      float daily_7_feels_like_eve = daily_7_feels_like["eve"]; // 6.66
      float daily_7_feels_like_morn = daily_7_feels_like["morn"]; // 3.47
      
      int daily_7_pressure = daily_7["pressure"]; // 1017
      int daily_7_humidity = daily_7["humidity"]; // 94
      float daily_7_dew_point = daily_7["dew_point"]; // 4.94
      float daily_7_wind_speed = daily_7["wind_speed"]; // 4.58
      int daily_7_wind_deg = daily_7["wind_deg"]; // 189
      float daily_7_wind_gust = daily_7["wind_gust"]; // 8.27
      
      JsonObject daily_7_weather_0 = daily_7["weather"][0];
      int daily_7_weather_0_id = daily_7_weather_0["id"]; // 500
      const char* daily_7_weather_0_main = daily_7_weather_0["main"]; // "Rain"
      const char* daily_7_weather_0_description = daily_7_weather_0["description"]; // "light rain"
      const char* daily_7_weather_0_icon = daily_7_weather_0["icon"]; // "10d"
      
      int daily_7_clouds = daily_7["clouds"]; // 100
      int daily_7_pop = daily_7["pop"]; // 1
      float daily_7_rain = daily_7["rain"]; // 3.27
      int daily_7_uvi = daily_7["uvi"]; // 3
      */
  
      Serial.printf("%d precipitation points parsed\n", data.precipitation.size());  
      Serial.printf("Outside temperature %.1f C\n", data.outsideTemp);
    
   } else {
      Serial.printf("httpCode: %d != %d\n", httpCode, HTTP_CODE_OK);
      return; // httpCode == HTTP_CODE_OK
   }
  } else {
      Serial.printf("httpCode: %d <= 0", httpCode);
      return; // httpCode > 0
  }

  // Everything went well
  Serial.println("Weather request went well");
  snprintf( data.displayTemperature, sizeof( data.displayTemperature ), "%0d%sC", (int) data.outsideTemp, DEGREE_SYMBOL );
  data.lastWeatherUpdate = time( NULL );
  data.weatherUpdated = true;
} // getWeather
