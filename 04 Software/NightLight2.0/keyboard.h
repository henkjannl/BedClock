#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "data.h"

using namespace std;

/*   This file offers a global struct keyboard which offers
 *   three booleans. If a key is touched, a boolean is set.
 *   The main program must check the booleans and reset them 
 *   using keyboardMux
 */  
 
//=========== DEFINES AND CONSTANTS ==================//
#define PIN_KEY_LEFT   4
#define PIN_KEY_RIGHT 12
#define PIN_KEY_TOP   14
#define KEY_TRESHOLD  50

//==================== TYPES ====================//
class tKeyboard {
  public:
    volatile bool switchButtonPressed;
    volatile bool nextButtonPressed;
    volatile bool selectButtonPressed;

    volatile unsigned long keyboardAlive;

    tKeyboard() {
      switchButtonPressed=false;
      nextButtonPressed=false;
      selectButtonPressed=false;      
    };
      
};

//============= GLOBAL DECLARATIONS =============//

hw_timer_t * keyboardTimer = NULL;
portMUX_TYPE keyboardMux = portMUX_INITIALIZER_UNLOCKED;
tKeyboard keyboard;

// Timeouts
volatile int16_t keyboardTimeout;
volatile int16_t powerTimer; 


//================== FUNCTIONS ==================//
void IRAM_ATTR onCheckKeys() {
  static volatile int keyLeftCounter  = 0;
  static volatile int keyRightCounter = 0;
  static volatile int keyTopCounter   = 0;
  
  if (touchRead(PIN_KEY_LEFT )<KEY_TRESHOLD) keyLeftCounter++;  else keyLeftCounter =0; 
  if (touchRead(PIN_KEY_RIGHT)<KEY_TRESHOLD) keyRightCounter++; else keyRightCounter=0; 
  if (touchRead(PIN_KEY_TOP  )<KEY_TRESHOLD) keyTopCounter++;   else keyTopCounter  =0; 

  portENTER_CRITICAL_ISR(&keyboardMux);
  keyboard.keyboardAlive++;
  if(keyLeftCounter ==3) { keyboard.nextButtonPressed=true;   keyboardTimeout = 20; }
  if(keyRightCounter==3) { keyboard.selectButtonPressed=true; keyboardTimeout = 20; }
  if(keyTopCounter  ==3) { keyboard.switchButtonPressed=true; keyboardTimeout = 20; }
  portEXIT_CRITICAL_ISR(&keyboardMux); 
}

void setupKeyboard() {
  keyboardTimer= timerBegin(0, 80, true);
  timerAttachInterrupt(keyboardTimer, &onCheckKeys, true);
  timerAlarmWrite(keyboardTimer, 100000, true); // Keys are checked 10 times per second
  timerAlarmEnable(keyboardTimer);
}

#endif // KEYBOARD_H
