#ifndef QUOTE_H

#define QUOTE_H

#include "data.h"
#include <string>
#include <ArduinoJson.h>

using namespace std;

TimerHandle_t timerQuote; // New quote requested if this timer expires

static void timerQuoteCallback( TimerHandle_t xTimer );
void taskQuote(void * parameter );
void getQuote();

void setupQuote() {

  xTaskCreatePinnedToCore(
    taskQuote,              // The function containing the task
    "taskQuote",            // Name for mortals
    12000,                  // Stack size 
    NULL,                   // Parameters
    1,                      // Priority, 0=lowest, 3=highest
    NULL,                   // Task handle
    ARDUINO_RUNNING_CORE);  // Core to run the task on

  timerQuote=xTimerCreate( "TimerQuote", 
                20*60*1000,           // Replace quote every twenty minutes
                pdTRUE,               // Auto repeat timer 
                0,                    // TimerID, unused
                timerQuoteCallback);  // Callback function as timer expires
 
  xTimerStart(timerQuote, 1000);
}; // setupQuote


static void timerQuoteCallback( TimerHandle_t xTimer ) {
  /*  Retrieving a quote is not directly implemented in the timer
   *  since the code requires more memory than the timer routine has available,
   *  so this results in a stack overflow. Now, retrieving the weather is implemented
   *  in a separate task, taskWeather, and the timer sends a command to this task through
   *  the weatherQueue
   */
   
  data.requestQuote=true;
  Serial.println("New quote requested");
}

void taskQuote(void * parameter ) {

  while(true) {

    if(data.requestQuote & data.connected) {
      portENTER_CRITICAL(&connectionMux);
      portENTER_CRITICAL(&dataAccessMux);
      getQuote();
      portEXIT_CRITICAL(&connectionMux);
      portEXIT_CRITICAL(&dataAccessMux);
    }

    data.quoteHighWaterMark= uxTaskGetStackHighWaterMark(NULL);
    vTaskDelay(1000);
    
  } // while true
} // taskQuote      

void getQuote() {
  
  HTTPClient http;

  http.begin("http://api.adviceslip.com/advice");
  
  int httpCode = http.GET();
  Serial.printf("http get return code %d\n", httpCode);
  
  if(httpCode > 0) {    
    if(httpCode == HTTP_CODE_OK) {
      StaticJsonDocument<1024> doc;
      
      DeserializationError error = deserializeJson(doc, http.getString());
      
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      
      //int slip_id = doc["slip"]["id"]; // 143
      data.quote = doc["slip"]["advice"].as<string>(); // "If you cannot unscrew the lid of a jar, try placing ...
      data.quoteAvailable=dqRefreshed;
      data.requestQuote=false;
      data.quoteAlive++;
      Serial.println(data.quote.c_str());
    } // httpCode == HTTP_CODE_OK
    else Serial.printf("HTTP_CODE not OK: httpCode = %d\n", httpCode);
  } // httpCode > 0
  else Serial.printf("Error: httpCode = %d0\n", httpCode);
} //

#endif // QUOTE_H
