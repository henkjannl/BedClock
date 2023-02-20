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


//============= GLOBAL DECLARATIONS =============//
TimerHandle_t keyboardTimer;   // Polls capacitive keys @30Hz
TimerHandle_t keyboardTimeoutTimer; // Menu goes back to main menu after 20 seconds


//================== FUNCTIONS ==================//
static void keyboardTimerCallback( TimerHandle_t xTimer ) {

  static volatile uint32_t keyLeftCounter  = 0;
  static volatile uint32_t keyRightCounter = 0;
  static volatile uint32_t keyTopCounter   = 0;

  tButtonClick whichButton;
  
  if (touchRead(PIN_KEY_LEFT )<KEY_TRESHOLD) keyLeftCounter++;  else keyLeftCounter =0; 
  if (touchRead(PIN_KEY_TOP  )<KEY_TRESHOLD) keyTopCounter++;   else keyTopCounter  =0; 
  if (touchRead(PIN_KEY_RIGHT)<KEY_TRESHOLD) keyRightCounter++; else keyRightCounter=0; 

  if(keyLeftCounter ==3) { whichButton=btnNext;   xQueueSendToBack( keyboardQueue, &whichButton, 0 ); xTimerReset( keyboardTimeoutTimer, 0); }
  if(keyTopCounter  ==3) { whichButton=btnPower;  xQueueSendToBack( keyboardQueue, &whichButton, 0 ); xTimerReset( keyboardTimeoutTimer, 0); }
  if(keyRightCounter==3) { whichButton=btnSelect; xQueueSendToBack( keyboardQueue, &whichButton, 0 ); xTimerReset( keyboardTimeoutTimer, 0); }

  data.keyboardAlive++;

}; // keyboardTimerCallback

static void keyboardTimeoutCallback( TimerHandle_t xTimer ) {
  tButtonClick whichButton=keyboardTimeout; 
  xQueueSendToBack( keyboardQueue, &whichButton, 0 );
}; // keyboardTimeoutCallback

void setupKeyboard() {  
  keyboardQueue=xQueueCreate(10,sizeof(tButtonClick)); 

  keyboardTimer=xTimerCreate( "Keys", 
                pdMS_TO_TICKS(30), // Routine called at 30 Hz, so key response at 10 Hz
                pdTRUE,            // Auto reload, 
                0,                 // TimerID, unused
                keyboardTimerCallback); 

  keyboardTimeoutTimer=xTimerCreate( "KeyTimeOut", 
                pdMS_TO_TICKS(20000), // Menu goes back to idle after 20 seconds
                pdFALSE  ,            // Single shot timer 
                0,                    // TimerID, unused
                keyboardTimeoutCallback); 
                  

  xTimerStart(keyboardTimer, 0 );
}; // setupKeyboard

#endif // KEYBOARD_H
