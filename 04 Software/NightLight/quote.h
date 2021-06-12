#ifndef QUOTE_H

#define QUOTE_H

#include "data.h"
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
          StaticJsonDocument<192> doc;
          
          DeserializationError error = deserializeJson(doc, http.getString());
          
          if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
          }
          
          //int slip_id = doc["slip"]["id"]; // 19
          string quote = doc["slip"]["advice"]; // "If you cannot unscrew the lid of a jar, try placing ...

          portENTER_CRITICAL(&dataAccessMux);
          data.quote1=quote;
          data.quote2="";
          uint8_t lengthDiff=data.quote1.length();

          for(int i=0; i<quote.length(); i++) {

            if(isspace(quote[i])) {
              if(i>(quote.length()-i)) {
                if(i-(quote.length()-i)<lengthDiff) {
                  data.quote1=quote.substr(0, i);
                  data.quote2=quote.substr(i+1);
                  lengthDiff=abs(data.quote1.length()-data.quote2.length());
                  Serial.printf("%d | %d diff %d\n", data.quote1.length(), data.quote2.length(), lengthDiff);
                } // i-quote.length()<lengthDiff
              } // i>quote.length()-i
            else { // i<quote.length()-i            
                if(quote.length()-2*i<lengthDiff) {
                  data.quote1=quote.substr(0, i);
                  data.quote2=quote.substr(i+1);
                  lengthDiff=abs(data.quote1.length()-data.quote2.length());
                  Serial.printf("%d | %d diff %d\n", data.quote1.length(), data.quote2.length(), lengthDiff);
                } // quote.length()-2*i<lengthDiff
              } // i<quote.length()-i
            } // whitespace

          } // for i
          Serial.println(data.quote1.c_str());
          Serial.println(data.quote2.c_str());
          
          data.quoteAvailable=dqRefreshed;
          data.requestQuote=false;
          data.quoteAlive++;
          portEXIT_CRITICAL(&dataAccessMux);
        }
      }
    } // requestQuote
    
    data.quoteHighWaterMark= uxTaskGetStackHighWaterMark(NULL);
    vTaskDelay(1000);
    
  } // while true
} // taskQuote      


#endif // QUOTE_H
