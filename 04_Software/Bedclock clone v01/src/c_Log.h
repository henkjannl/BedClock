#pragma once

#include "FS.h"
#include "SPIFFS.h"
#include <AsyncTelegram2.h>

#define LOG_FILE "/Logfile.csv"

#define FORMAT_SPIFFS_IF_FAILED true

void removeLogfile() {
  if(!USE_SPIFFS) return;
  SPIFFS.remove( LOG_FILE );
}

void newLogfile() {
  if(!USE_SPIFFS) return;

  if( SPIFFS.exists( LOG_FILE ) ) removeLogfile();

  File file = SPIFFS.open(LOG_FILE, FILE_APPEND);
  file.print( F("Date, Time, LastWeatherUpdate, outsideTemp, pressure, humidity, feelsLike, sunrise, sunset, maxDayTemp, maxTomorrowTemp, windSpeed, windDir, windDirStr\n" ) );
  file.close();
}

void addToLogfile( ) {
  if(!USE_SPIFFS) return;

  time_t rawtime;
  struct tm * timeinfo;
  char item[20];

  if( !SPIFFS.exists( LOG_FILE ) ) newLogfile();

  time (&rawtime);
  timeinfo = localtime (&rawtime);

  snprintf( item, sizeof(item), "%04d-%02d-%02d, ", 1900+timeinfo->tm_year, 1+timeinfo->tm_mon, timeinfo->tm_mday );
  String content = String( item );

  snprintf( item, sizeof(item), "%02d:%02d:%02d, ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
  content += String( item );

  timeinfo = localtime ( &(data.lastWeatherUpdate) );
  snprintf( item, sizeof(item), "%02d:%02d:%02d, ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec) ;
  content += String( item );

  snprintf( item, sizeof(item), "%f, ", data.outsideTemp );
  content += String( item );

  snprintf( item, sizeof(item), "%d, ", data.pressure );
  content += String( item );

  snprintf( item, sizeof(item), "%d, ", data.humidity );
  content += String( item );

  snprintf( item, sizeof(item), "%f, ", data.outsideFeelsLike );
  content += String( item );

  timeinfo = localtime ( &(data.daySunrise) );
  snprintf( item, sizeof(item), "%02d:%02d:%02d, ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec) ;
  content += String( item );

  timeinfo = localtime ( &(data.daySunset) );
  snprintf( item, sizeof(item), "%02d:%02d:%02d, ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec) ;
  content += String( item );

  snprintf( item, sizeof(item), "%f, ", data.maxDayTemp );
  content += String( item );

  snprintf( item, sizeof(item), "%f, ", data.maxTomorrowTemp );
  content += String( item );

  snprintf( item, sizeof(item), "%f, ", data.windspeed );
  content += String( item );

  snprintf( item, sizeof(item), "%d, ", data.windDirection );
  content += String( item );

  content += data.windDirStr + "\n";

  File file = SPIFFS.open(LOG_FILE, FILE_APPEND);
  file.print( content );
  file.close();
} 

void sendLogFile(const TBMessage &queryMsg) {
  /*
  File file = SPIFFS.open(LOG_FILE, "r");
  if (file) {
    myBot.sendDocument(queryMsg, file, file.size(), AsyncTelegram2::DocumentType::CSV, file.name());
    file.close();
  }
  else
    Serial.println("Can't open the file");
  */  
}

void listDir(String & result, const char * dirname, uint16_t level = 0 ) {
  if(!USE_SPIFFS) return;

  result += "Listing directory: " + String( dirname ) + "\n";

  File root = SPIFFS.open(dirname);
  if(!root){
      result + "- failed to open directory";
      return;
  }
  if(!root.isDirectory()){
      result + " - not a directory";
      return;
  }

  File file = root.openNextFile();
  while(file){
    for(int i=0; i<level; i++) result+="  ";
    if(file.isDirectory()){
      result += "> " + String( file.name() ) + "\n";
      listDir(result, file.name(), level+1 );
    } else {
      result += String( file.name() ) + "  " + String( (int) file.size() ) + "\n";
    }
    file = root.openNextFile();
  }
}
