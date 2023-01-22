#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "a_data.h"

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
volatile uint32_t keyboardEnableCounter = 0;

//================== FUNCTIONS ==================//
void enableKeyboard() {
  keyboardEnableCounter--;
};

void disableKeyboard() {
  keyboardEnableCounter++;
};

bool keyboardEnabled() {
  return (keyboardEnableCounter==0);
};

static void keyboardTimerCallback( TimerHandle_t xTimer ) {

  static volatile uint32_t keyLeftCounter  = 0;
  static volatile uint32_t keyRightCounter = 0;
  static volatile uint32_t keyTopCounter   = 0;

  if( keyboardEnabled() ) {
    if (touchRead(PIN_KEY_LEFT )<KEY_TRESHOLD) keyLeftCounter++;  else keyLeftCounter =0; 
    if (touchRead(PIN_KEY_TOP  )<KEY_TRESHOLD) keyTopCounter++;   else keyTopCounter  =0; 
    if (touchRead(PIN_KEY_RIGHT)<KEY_TRESHOLD) keyRightCounter++; else keyRightCounter=0; 
    
    if(keyLeftCounter ==2) { sendCommandToQueue( cmdButtonLeft,  screenQueue ); }; 
    if(keyTopCounter  ==2) { sendCommandToQueue( cmdLightToggle, lightQueue  ); };
    if(keyRightCounter==2) { sendCommandToQueue( cmdButtonRight, screenQueue ); }; 
    
    data.keyboardAlive++;
  };

}; // keyboardTimerCallback

void setupKeyboard() {  

  keyboardTimer=xTimerCreate( "Keys", 
                pdMS_TO_TICKS(50), // Routine called at 20 Hz, so key response at 10 Hz
                pdTRUE,            // Auto reload, 
                0,                 // TimerID, unused
                keyboardTimerCallback); 
                  
  xTimerStart(keyboardTimer, 0 );
}; // setupKeyboard

#endif // KEYBOARD_H
