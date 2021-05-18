#ifndef ONESECTIMER_H
#define ONESECTIMER_H

#include "data.h"
#include "keyboard.h"

using namespace std;

//   Simple one sec timer which counts down the timers  
hw_timer_t * oneSecTimer = NULL;

//================== FUNCTIONS ==================//
void IRAM_ATTR onOneSecTimer() {
  portENTER_CRITICAL_ISR(&keyboardMux);
  if(keyboardTimeout>0) keyboardTimeout--;
  if(powerTimer>0)      powerTimer--;  
  portEXIT_CRITICAL_ISR(&keyboardMux);
}

void setupOneSecTimer() {
  oneSecTimer = timerBegin(1, 80, true);
  timerAttachInterrupt(oneSecTimer, &onOneSecTimer, true);
  timerAlarmWrite(oneSecTimer, 1000000, true); 
  timerAlarmEnable(oneSecTimer);
}

#endif // ONESECTIMER_H
