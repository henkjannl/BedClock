#ifndef QUOTE_H

#define QUOTE_H

#include "a_data.h"
#include <string>
#include <ArduinoJson.h>

using namespace std;

TimerHandle_t timerQuote; // New quote requested if this timer expires

static void timerQuoteCallback( TimerHandle_t xTimer );
void taskQuote(void * parameter );

void setupQuote() {

  data.requestQuote=true;

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
            return;
          }
          
          portENTER_CRITICAL(&dataAccessMux);
          //int slip_id = doc["slip"]["id"]; // 19
          data.quote = doc["slip"]["advice"].as<string>(); // "If you cannot unscrew the lid of a jar, try placing ...
          data.quoteAvailable=dqRefreshed;
          data.requestQuote=false;
          data.quoteAlive++;
          Serial.println(data.quote.c_str());
          portEXIT_CRITICAL(&dataAccessMux);
        }
      }
    } // requestQuote
    
    data.quoteHighWaterMark= uxTaskGetStackHighWaterMark(NULL);
    vTaskDelay(1000);
    
  } // while true
} // taskQuote      


#endif // QUOTE_H
