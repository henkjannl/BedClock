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
    2,                      // Priority, 0=lowest, 3=highest
    NULL,                   // Task handle
    ARDUINO_RUNNING_CORE);  // Core to run the task on

}

void taskScreen(void * parameter ){

  while(true) {

      if(data.switchButtonPressed) {
        portENTER_CRITICAL(&dataAccessMux);
        data.switchButtonPressed=false;
        data.powerState = (data.powerState==psTimer) ? psOff : psTimer;
        portEXIT_CRITICAL(&dataAccessMux);
      };
      
      if(data.nextButtonPressed) {
        portENTER_CRITICAL(&dataAccessMux);
        data.nextButtonPressed=false;
        portEXIT_CRITICAL(&dataAccessMux);
        display.nextButton();
      };
      
      if(data.selectButtonPressed) {
        portENTER_CRITICAL(&dataAccessMux);
        data.selectButtonPressed=false;
        portEXIT_CRITICAL(&dataAccessMux);
        display.selectButton(data);
      };

    u8g2.setDrawColor(0);
    u8g2.clearBuffer();  
    display.display(u8g2);
    u8g2.sendBuffer();

    display.step();

    vTaskDelay(20);
  };
};

#endif // SCREEN_H
