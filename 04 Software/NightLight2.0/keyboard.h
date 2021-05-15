#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "data.h"
#include "key.h"

using namespace std;


// ======== GLOBAL VARIABLES ============= 
Key nextButton  ( 4, 50);
Key selectButton(12, 50);
Key switchButton(14, 50);

void taskKeyboard(void * parameter );

void setupKeyboard() {
  
  xTaskCreatePinnedToCore(
    taskKeyboard,           // The function containing the task
    "TaskKeyboard",         // Name for mortals
    12000,                  // Stack size 
    NULL,                   // Parameters
    3,                      // Priority, 0=lowest, 3=highest
    NULL,                   // Task handle
    ARDUINO_RUNNING_CORE);  // Core to run the task on

}

void taskKeyboard(void * parameter ){

  while(true) {

    switchButton.scan();
    nextButton  .scan();
    selectButton.scan();
    
    if(switchButton.pressed()) {
      portENTER_CRITICAL(&dataAccessMux);
      data.switchButtonPressed=true;
      portEXIT_CRITICAL(&dataAccessMux);
    };
    
    if(nextButton.pressed()) {
      portENTER_CRITICAL(&dataAccessMux);
      data.nextButtonPressed=true;
      portEXIT_CRITICAL(&dataAccessMux);
    };
    
    if(selectButton.pressed()) {
      portENTER_CRITICAL(&dataAccessMux);
      data.selectButtonPressed=true;
      portEXIT_CRITICAL(&dataAccessMux);
    };

    vTaskDelay(2);
  };
};

#endif // KEYBOARD_H
