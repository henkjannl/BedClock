#ifndef SCREEN_H
#define SCREEN_H

#include "data.h"
#include "display.h"

#include <U8g2lib.h>
#include <Wire.h>
#include <math.h>       

using namespace std;


// ======== GLOBAL CONSTANTS ============= 
# define PI 3.141592653589793238462643383279502884L /* pi */

// ======== GLOBAL VARIABLES ============= 
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R2, 22, 21, U8X8_PIN_NONE);  

TimerHandle_t timerScreenContrast; // Adjust the screen contrast periodically
bool triggerScreenContrastAdjustment;

// ======== FUNCTION DECLARATIONS ============= 
static void timerScreenContrastCallback( TimerHandle_t xTimer );
void taskScreen(void * parameter );

void setupScreen() {

  // Initialize display
  u8g2.initDisplay();
  delay(500);
  u8g2.begin();
  
  xTaskCreatePinnedToCore(
    taskScreen,             // The function containing the task
    "TaskScreen",           // Name for mortals
    12000,                  // Stack size 
    NULL,                   // Parameters
    1,                      // Priority, 0=lowest, 3=highest
    NULL,                   // Task handle
    ARDUINO_RUNNING_CORE);  // Core to run the task on

  timerScreenContrast=xTimerCreate( "TimerScreenContrast", 
                10*60*1000,                   // Adjust screen contrast every 10 minutes
                pdTRUE,                       // Auto repeat timer
                0,                            // TimerID, unused
                timerScreenContrastCallback); // Callback function as timer expires

}; // setupScreen

static void timerScreenContrastCallback( TimerHandle_t xTimer ){
  triggerScreenContrastAdjustment=true;
}

void setScreenBrightness(uint8_t contrast) {
  /* Typical values for the parameters contrast, vcom, p1, p2:
   *  255, 0,  1, 1 // most bright
   *   16, 0,  1, 1
   *    5, 0,  0, 0
   *    3, 0,  0, 0
   *    1, 0,  0, 0 // least bright   */

  uint8_t vcom = 0;
  uint8_t p1 = contrast > 15 ? 1 : 0; 
  uint8_t p2 = contrast > 15 ? 1 : 0;

  u8g2.setContrast(contrast);
  u8x8_cad_StartTransfer(u8g2.getU8x8());
  u8x8_cad_SendCmd(u8g2.getU8x8(), 0x0db);
  u8x8_cad_SendArg(u8g2.getU8x8(), vcom << 4);
  u8x8_cad_SendCmd(u8g2.getU8x8(), 0x0d9);
  u8x8_cad_SendArg(u8g2.getU8x8(), (p2 << 4) | p1 );
  u8x8_cad_EndTransfer(u8g2.getU8x8());
}

void taskScreen(void * parameter ) {

  button_t keystroke;
  BaseType_t buttonRead;
  BaseType_t success;
  tMenuItem command;

  static bool autoScreenContrast = true;
  triggerScreenContrastAdjustment = true;
  
  // Send default settings to light task
  command=cmdDuration10;   xQueueSendToBack(lightQueue, &command, 0 );    
  command=cmdColorWhite;   xQueueSendToBack(lightQueue, &command, 0 );    
  command=cmdBrightness40; xQueueSendToBack(lightQueue, &command, 0 );    
  command=cmdLightOn;      xQueueSendToBack(lightQueue, &command, 0 );    

  // Set default contrast
  u8g2.setContrast(255); // Perhaps remove later, just to test if the contrast function works

  while(true) {
    buttonRead=xQueueReceive(keyboardQueue, (void *) &keystroke, 0); 
    switch (buttonRead) {

      case btnNext:
        Serial.println("Next button pressed");
      break;

      case btnSelect: 
        Serial.println("Select button pressed");
      break;

      case btnLight:
        // Switch power on off    
        command=cmdLightToggle;
        xQueueSendToBack( lightQueue, &command, 0 );    
        Serial.println("Light button pressed");
      break;
    }; // switch buttonRead

    if(autoScreenContrast & triggerScreenContrastAdjustment) {
      Serial.println("Auto screen brightness timer triggered");

      if(data.weatherAvailable==dqUnavailable) {
        Serial.println("Sunrise and sunset are not yet available");
        setScreenBrightness( 1 ); // least bright   */
      }
      else if(!data.syncTime) {
        Serial.println("Time not yet synched");
        setScreenBrightness(  1 ); // least bright   */
      }
      else {        
        time_t currentTime;
        time(&currentTime);
        
        struct tm * timeinfo;
        timeinfo = localtime(&data.sunrise);  Serial.printf("Sunrise: %s",      asctime(timeinfo));
        timeinfo = localtime(&currentTime);   Serial.printf("Current time: %s", asctime(timeinfo));
        timeinfo = localtime(&data.sunset);   Serial.printf("Sunset: %s",       asctime(timeinfo));

        /* REMARK:
         *  This approach is only valid if sunset and sunrise are 12 hours apart. If 
         *  the day is much longer or shorter, a more advanced approach is preferred */
        float dayLength=data.sunset-data.sunrise;
        float sunStrength=sin(PI*(currentTime-data.sunrise)/dayLength);

        Serial.printf("Day length: %.3f\n", dayLength);
        Serial.printf("Sun strength: %.3f\n", sunStrength);
  
             if(sunStrength> 0.40) setScreenBrightness( 255 ); // most bright
        else if(sunStrength> 0.13) setScreenBrightness(  16 );
        else if(sunStrength>-0.13) setScreenBrightness(   5 );
        else if(sunStrength>-0.40) setScreenBrightness(   3 );
        else                       setScreenBrightness(   1 ); // least bright   */
        
        triggerScreenContrastAdjustment=false;
      }
    }
    uint32_t redrawBegin=millis();

    u8g2.setDrawColor(0);
    u8g2.clearBuffer();  
    /*
    Implement drawing the display here
    */
    u8g2.sendBuffer();

    uint32_t redrawReady=millis();
    
    /* Keep a watch on how much memory is used by the measurement thread */
    data.screenHighWaterMark= uxTaskGetStackHighWaterMark(NULL);
    data.screenAlive++;
    data.screenRedrawMillis=redrawReady-redrawBegin;
    
    vTaskDelay(50);
  }; // while true
}; // taskscreen

#endif // SCREEN_H
