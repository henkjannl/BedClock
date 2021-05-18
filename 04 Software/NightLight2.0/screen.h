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

      if(keyboardTimeout==0) {
        display.selectedRow=-1;
        display.showMain();
      };

      if( (powerTimer==0) && (data.timerDuration!=tdOff) ) data.powerState = psOff;
      
      if(keyboard.switchButtonPressed) {
        portENTER_CRITICAL(&keyboardMux);
        
        keyboard.switchButtonPressed=false;
        
        if     (data.timerDuration==td03) powerTimer=3*60;
        else if(data.timerDuration==td05) powerTimer=5*60;
        else if(data.timerDuration==td10) powerTimer=10*60;
        else if(data.timerDuration==td20) powerTimer=20*60;
        
        portEXIT_CRITICAL(&keyboardMux);

        portENTER_CRITICAL(&dataAccessMux);
        data.powerState = (data.powerState==psTimer) ? psOff : psTimer;
        portEXIT_CRITICAL(&dataAccessMux);
      };
      
      if(keyboard.nextButtonPressed) {

        portENTER_CRITICAL(&keyboardMux);
        keyboard.nextButtonPressed=false;
        portEXIT_CRITICAL(&keyboardMux);
        
        display.nextButton();
      };
      
      if(keyboard.selectButtonPressed) {
        portENTER_CRITICAL(&keyboardMux);
        keyboard.selectButtonPressed=false;
        portEXIT_CRITICAL(&keyboardMux);

        display.selectButton(data);
      };

    u8g2.setDrawColor(0);
    u8g2.clearBuffer();  
    display.display(u8g2);
    u8g2.sendBuffer();

    display.step();

    portENTER_CRITICAL(&dataAccessMux);
    /* Keep a watch on how much memory is used by this thread */
    data.screenHighWaterMark= uxTaskGetStackHighWaterMark(NULL);
    data.screenAlive++;
    portEXIT_CRITICAL(&dataAccessMux);
    
    vTaskDelay(100);
  };
};

#endif // SCREEN_H
