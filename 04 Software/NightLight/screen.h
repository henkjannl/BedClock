#ifndef SCREEN_H
#define SCREEN_H

#include "data.h"
#include "display.h"

#include <U8g2lib.h>
#include <Wire.h>

using namespace std;

// ======== GLOBAL VARIABLES ============= 
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R2, 22, 21, U8X8_PIN_NONE);  
tDisplay display(u8g2);

// ======== FUNCTION DECLARATIONS ============= 
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
}; // setupScreen

void taskScreen(void * parameter ) {

  tButtonClick keystroke;
  BaseType_t buttonRead;
  BaseType_t success;
  tMenuItem command;

  while(true) {
    buttonRead=xQueueReceive(keyboardQueue, (void *) &keystroke, 0); 
    if (buttonRead) {

      if(keystroke==keyboardTimeout) {
        // Keyboard timeout, switch back to main screen
        display.showMain();
      }
      else if (keystroke==btnPower) {
        // Switch power on off    
        Serial.println("Power button pressed");
        command=mainTogglePower;
        xQueueSendToBack( lightQueue, &command, 0 );    
      }
      else if (keystroke==btnNext) {
        Serial.println("Next button pressed");
        display.nextButton();
      }
      else if (keystroke==btnSelect) {
        // Notify the light that the user selected a command
        Serial.println("Select button pressed");
        command = display.selectButton();
        xQueueSendToBack(lightQueue, &command, 0 );
      }
    }; // buttonRead

    uint32_t redrawBegin=millis();

    u8g2.setDrawColor(0);
    u8g2.clearBuffer();  
    display.display(u8g2);
    u8g2.sendBuffer();

    uint32_t redrawReady=millis();

    display.step();
    
    /* Keep a watch on how much memory is used by the measurement thread */
    data.screenHighWaterMark= uxTaskGetStackHighWaterMark(NULL);
    data.screenAlive++;
    data.screenRedrawMillis=redrawReady-redrawBegin;
    
    vTaskDelay(100);
  }; // while true
}; // taskscreen

#endif // SCREEN_H
