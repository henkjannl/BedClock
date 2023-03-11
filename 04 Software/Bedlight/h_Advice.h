#pragma once

#include "f_Display.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <list>

String advice() {

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

void getAdvice() {
  // Get a new advice from the internet
  String newAdvice = advice();

  // Try how it fits best on the screen
  Serial.printf( "** NEW ADVICE %s **\n", newAdvice.c_str() );

  // Check if the whole advice fits on one line
  if( adviceFitsOnScreen( newAdvice ) ) {
    data.newAdviceRequested = false;
    data.displayAdviceLines = 1;
    snprintf( data.displayAdvice[0], sizeof( data.displayAdvice[0]), newAdvice.c_str() );
    Serial.println( "Advice fits in one line" );
  } 
  else {
    
    // Try to separate the words equally over two lines
    int indexOfSplit = newAdvice.indexOf(" ");

    int bestSplit = -1;
    int differenceBetweenLines = 1000;

    while( indexOfSplit != -1 ) {
      int widthOfLine1 = widthOfAdviceString( newAdvice.substring( 0, indexOfSplit ) );
      int widthOfLine2 = widthOfAdviceString( newAdvice.substring( indexOfSplit+1 ) );

      Serial.printf( "Trying [%s] and [%s]\n", newAdvice.substring( 0, indexOfSplit ).c_str(), newAdvice.substring( indexOfSplit+1 ).c_str() );
      Serial.printf( "Width %d and %d\n", widthOfLine1, widthOfLine2 );

      // Check if this split offers a valid result
      if( ( abs( widthOfLine2 - widthOfLine1 ) < differenceBetweenLines ) and     // Best split so far
          adviceFitsOnScreen( newAdvice.substring( 0, indexOfSplit ) )      and     // Line 1 fits on the screen
          adviceFitsOnScreen( newAdvice.substring( indexOfSplit+1  ) )          ) { // Line 2 fits on the screen

        bestSplit = indexOfSplit;
        differenceBetweenLines = abs( widthOfLine2 - widthOfLine1 );
        Serial.printf("Best split for far, difference is %d\n", differenceBetweenLines );

      } // valid split and best so far

      indexOfSplit = newAdvice.indexOf(" ", indexOfSplit+1 );

    } // while( indexOfSplit != -1 ) {
    
    if( bestSplit>0 ) {
      data.newAdviceRequested = false;
      data.displayAdviceLines = 2;
      snprintf( data.displayAdvice[0], sizeof( data.displayAdvice[0]), newAdvice.substring( 0, bestSplit ).c_str() );
      snprintf( data.displayAdvice[1], sizeof( data.displayAdvice[1]), newAdvice.substring( bestSplit+1  ).c_str() );
    } // if( bestSplit>0 ) {


  } // adviceFitsOnScreen( newAdvice ) else
}
