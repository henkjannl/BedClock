#ifndef WEATHER_H

#define WEATHER_H

#include "data.h"
#include <ArduinoJson.h>

void requestWeather(tInternalData& internalData) {
  // If the weather was recently retrieved, go back
  if (internalData.weather.quality==dqFresh) return;
  if(!internalData.locationKnown) return;

  Serial.println(F("Retrieving the weather"));
    
  HTTPClient http;

  char buff[200];
  snprintf(buff, sizeof(buff), "https://api.openweathermap.org/data/2.5/onecall?lat=%.8f&lon=%.8f&exclude=hourly,daily,alerts&appid=%s", 
    data.lat, data.lon, config.OpenWeatherMapApiKey);
  
  Serial.println(buff);

  http.begin(buff);
  /*
  switch(internalData.location) {
    case lcEnschede:
      http.begin(F("https://api.openweathermap.org/data/2.5/onecall?lat=52.23852421929482&lon=6.8467683083413595&exclude=hourly,daily,alerts&appid=f7f783df9efa41593710220c21878d4b"));
      Serial.println("Enschede");
      break;
      
    default:
      http.begin(F("https://api.openweathermap.org/data/2.5/onecall?lat=52.25325217357761&lon=6.785769056739125&exclude=hourly,daily,alerts&appid=f7f783df9efa41593710220c21878d4b"));
      Serial.println("Hengelo");
  }
  */
  
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
      //p.prec= rand() % 23 + 1;
      internalData.precipitation.push_back(p);
    }

    data.weather.age=0;
    data.weather.modified=true;

    Serial.printf("Outside temperature %.1f C\n", internalData.outsideTemp);
   }    
  }
}

void shiftPrecipitation(tInternalData& internalData) {
  // Remove old precipitation data from the list
  time_t t;
  time(&t);
  bool oldprep=true;
  while (oldprep) {
    tPrecipitation p=internalData.precipitation.front();
    if (p.t<t) {
      internalData.precipitation.pop_front();
      internalData.weather.modified=true;
    }
    else {
      oldprep=false;
    }
  }
}

void displayWeather(tInternalData& internalData, TFT_eSPI& tft) {

  // Do not display the weather until we know what time it is
  if (internalData.timeSync.quality==dqUnavailable) return;

  // If the weather was not yet retrieved, go back
  if (internalData.weather.quality==dqUnavailable) return;

  // If the weather is unchanged since the last time, go back
  if (!(internalData.weather.modified)) return;

  Serial.println(F("Display the weather"));
    
  // Viewport screen for precipitation graph
  tft.fillRoundRect(0,104,132,40,7,CLR_DARK);
  tft.setViewport(5, 107, 126, 35, true);

  int x=0;
  for (tPrecipitation & p : internalData.precipitation) {
    //int h=18.0*log(p.prec+1); // Curve through (0,0) (0.25,3) (3,25)
    int h=36.0*log(p.prec+1); // Curve through (0,0) (0.25,8) (3,50)
    if (h<34) {
      tft.drawFastVLine(x++, 34-h, h, CLR_RAIN);
      tft.drawFastVLine(x++, 34-h, h, CLR_RAIN);
    }
    else {
      tft.drawFastVLine(x++, 0, 34, CLR_RAIN_SAT);
      tft.drawFastVLine(x++, 0, 34, CLR_RAIN_SAT);
    }
  }
  tft.drawFastHLine(0, 34, 126, CLR_BACKGROUND);

  tft.resetViewport();  

  // Viewport for temperature
  drawBmp(tft, "/Temper.bmp", 136, 104);
  tft.setViewport(139, 107, 54, 34, true);
  tft.setTextDatum(MR_DATUM);
  tft.setTextColor(CLR_DATE);
  tft.setFreeFont(FF21);
  char buffer[12];
  sprintf(buffer,"%.1f",data.outsideTemp); 
  tft.drawString(buffer,37,15);
  tft.resetViewport();  

  // Weather icon
  sprintf (buffer, "/%s.bmp", data.weatherIcon);
  drawBmp(tft, buffer, 200, 104);
  
  data.weather.modified=false;
}


#endif WEATHER_H
