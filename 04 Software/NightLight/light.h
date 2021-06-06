#ifndef LIGHT_H
#define LIGHT_H

#if CONFIG_FREERTOS_UNICORE
  #define ARDUINO_RUNNING_CORE 0
#else
  #define ARDUINO_RUNNING_CORE 1
#endif

#define TIMER_PERIOD_03 pdMS_TO_TICKS( 3*60*1000) 
#define TIMER_PERIOD_05 pdMS_TO_TICKS( 5*60*1000) 
#define TIMER_PERIOD_10 pdMS_TO_TICKS(10*60*1000) 
#define TIMER_PERIOD_20 pdMS_TO_TICKS(20*60*1000) 

#include "data.h"
#include <Freenove_WS2812_Lib_for_ESP32.h>

using namespace std;

// ======== CONSTANTS ============= 
const float LIGHT_INC[] = { 0.0008, 0.0056, 0.0148, 0.0274, 0.0422, 0.0578, 0.0726, 0.0852, 0.0944, 0.0992, 0.0992, 
  0.0944, 0.0852, 0.0726, 0.0578, 0.0422, 0.0274, 0.0148, 0.0056, 0.0008};

const int MAX_INC = 19;

typedef struct { float R,G,B; } color_t;

const float MAX_INTENSITY = 255.0;
const color_t COLORS[]  = { { 1.000, 1.000, 1.000 },   // White
                            { 1.000, 1.000, 0.500 },   // Yellow
                            { 1.000, 0.750, 0.500 },   // Orange
                            { 1.000, 0.500, 0.500 } }; // Red

const uint16_t SUB_BRIGHTNESS[] = {0x0000, 0x0008, 0x0208, 0x020A, 0x0A0A, 0x2A0A, 0x2A8A, 0x2AAA, 0xAAAA, 
    0xAABA, 0xABBA, 0xABBE, 0xEBBE, 0xEBBF, 0xFBBF, 0xFBFF };


// This class is used to smoothly control the values for the RGB channels of the light
class tFloatVariable {

  public:
    tFloatVariable (){ 
      _value=0;
      _amplitude=0;
      _index=MAX_INC+1;
      _ready=true;
    };    
    
    float getValue()  { return _value; };
    float getTarget() { return _target; };

    // Directly set value without steps
    void setValue(float value){ 
      _value=value; 
      _target=value;
      _ready=true;
    };

    // Go to target with motion steps
    void setTarget(float target) { 
      if( abs(target-_target)>0.001) { // New target is significantly different from previous target
        _amplitude=target-_value;
        _target=target;
        _index=0;
        _ready=false;
      };
    };

    // Do next step
    void step() { 
      if(!_ready) { 
        _value+=_amplitude*LIGHT_INC[_index]; 
        _index++; 
        if(_index>MAX_INC) _ready=true;
      }; // !_ready
    };

    bool ready() { return _ready; };

  private:
    float _value;
    float _target;
    float _amplitude;
    byte _index;
    bool _ready; 
}; // class tFloatVariable

// ======== GLOBAL VARIABLES ============= 
#define LED_COUNT   16
#define LED_PIN     23
#define LED_CHANNEL  0

Freenove_ESP32_WS2812 strip;

TimerHandle_t timerTimeoutTimer; // Light goes out if this timer is expired

void taskLight(void * parameter );

static void timerTimeoutCallback( TimerHandle_t xTimer ) {
  tMenuItem command=mainTimerExpired; 
  xQueueSendToBack( lightQueue, &command, 0 );
}; // timerTimeoutCallback

void setupLight() {
  strip = Freenove_ESP32_WS2812(LED_COUNT, LED_PIN, LED_CHANNEL, TYPE_GRB);
  strip.begin();
  strip.setBrightness(255);  

  lightQueue=xQueueCreate(10,sizeof(tMenuItem)); 

  timerTimeoutTimer=xTimerCreate( "TimerTimeOut", 
                TIMER_PERIOD_03,        // Menu goes back to idle after 20 seconds
                pdFALSE  ,              // Single shot timer 
                0,                      // TimerID, unused
                timerTimeoutCallback);  // Callback function as timer expires

  xTaskCreatePinnedToCore(
    taskLight,              // The function containing the task
    "TaskLight",            // Name for mortals
    12000,                  // Stack size 
    NULL,                   // Parameters
    1,                      // Priority, 0=lowest, 3=highest
    NULL,                   // Task handle
    ARDUINO_RUNNING_CORE);  // Core to run the task on

}; // setupLight

void taskLight(void * parameter ){
  
  static tFloatVariable R,G,B; // Individual color components
  static float brightness=0.25;
  static uint8_t colorIndex=0;  
  static bool powerState=true;
  static bool ignoreTimer=false;

  static tMenuItem command;
  static BaseType_t commandReceived;

  while(true) {

    commandReceived=xQueueReceive(lightQueue, (void *) &command, 0); 

    if(commandReceived) {
      switch(command) {

        case mainScreen:
          // command can be ignored
        break;

        case mainFull:
          // command can be ignored
        break;

        case mainNight:
          // command can be ignored
        break;

        case mainEmpty:
          // command can be ignored
        break;

        case mainPowerOn:
          Serial.println("Power on");
          powerState=true;
          if(powerState) {
            xTimerReset( timerTimeoutTimer, 0 ); 
          }
        break;
        
        case mainTogglePower:
          Serial.println("Power toggle");
          powerState=!powerState;
          if(powerState) {
            xTimerReset( timerTimeoutTimer, 0 ); 
          }
        break;

        case mainTimerExpired:
          Serial.println("Timer expired");
          if(!ignoreTimer) powerState=false;
        break;
        
        case mainBrightness:
          // command can be ignored
        break;
        
        case mainColor:
          // command can be ignored
        break;
        
        case mainTimer:
          // command can be ignored
        break;
        
        case mainBack:
          // command can be ignored
        break;
        
        case brightness25:
          Serial.println("Brightness 25%");
          brightness=0.25;
        break;
        
        case brightness35:
          Serial.println("Brightness 35%");
          brightness=0.35;
        break;
        
        case brightness50:
          Serial.println("Brightness 50%");
          brightness=0.50;
        break;
        
        case brightness70:
          Serial.println("Brightness 70%");
          brightness=0.70;
        break;
        
        case brightness100:
          Serial.println("Brightness 100%");
          brightness=1.00;
        break;
        
        case brightnessBack:
          // command can be ignored
        break;
        
        case colorWhite:
          Serial.println("Color white");
          colorIndex=0;
        break;
        
        case colorYellow:
          Serial.println("Color yellow");
          colorIndex=1;
        break;
        
        case colorOrange:
          Serial.println("Color orange");
          colorIndex=2;
        break;
        
        case colorRed:
          Serial.println("Color red");
          colorIndex=3;
        break;
        
        case colorBack:
          // command can be ignored
        break;
        
        case timer03:
          Serial.println("Timer 3 min");
          xTimerChangePeriod(timerTimeoutTimer, TIMER_PERIOD_03, 0); 
          xTimerReset(timerTimeoutTimer, 0 ); // ToDO: not sure if this is needed
          ignoreTimer=false;
        break;
        
        case timer05:
          Serial.println("Timer 5 min");
          xTimerChangePeriod(timerTimeoutTimer, TIMER_PERIOD_05, 0); 
          xTimerReset(timerTimeoutTimer, 0 ); // ToDO: not sure if this is needed
          ignoreTimer=false;
        break;
        
        case timer10:
          Serial.println("Timer 10 min");
          xTimerChangePeriod(timerTimeoutTimer, TIMER_PERIOD_10, 0); 
          xTimerReset(timerTimeoutTimer, 0 ); // ToDO: not sure if this is needed
          ignoreTimer=false;
        break;
        
        case timer20:
          Serial.println("Timer 20 min");
          xTimerChangePeriod(timerTimeoutTimer, TIMER_PERIOD_20, 0); 
          xTimerReset(timerTimeoutTimer, 0 ); // ToDO: not sure if this is needed
          ignoreTimer=false;
        break;
        
        case timerOff:
          Serial.println("Timer off");
          ignoreTimer=true;
        break;
        
        case timerBack:
          // command can be ignored
        break;
      } // switch command
    } // if command received
        
    if(powerState) {
      // If the user has changed bridghtess or color sett
      R.setTarget(MAX_INTENSITY*COLORS[colorIndex].R*brightness);
      G.setTarget(MAX_INTENSITY*COLORS[colorIndex].G*brightness);
      B.setTarget(MAX_INTENSITY*COLORS[colorIndex].B*brightness);
    }
    else {
      R.setTarget(0);
      G.setTarget(0);
      B.setTarget(0);
    } // if powerState

    // Get the integer component
    uint16_t bR=(uint16_t) R.getValue();
    uint16_t bG=(uint16_t) G.getValue();
    uint16_t bB=(uint16_t) B.getValue();

    // Translate into sublevels for the individual levels
    uint8_t  baseR = bR>>4;  uint16_t subR = SUB_BRIGHTNESS[bR & 0xF]; 
    uint8_t  baseG = bG>>4;  uint16_t subG = SUB_BRIGHTNESS[bG & 0xF]; 
    uint8_t  baseB = bB>>4;  uint16_t subB = SUB_BRIGHTNESS[bB & 0xF]; 

    uint8_t ledR, ledG, ledB;
    uint16_t controlBit=0x01;
   
    for(uint8_t i=0; i<16; i++) {
        ledR=baseR;
        ledG=baseG; 
        ledB=baseB;

        if( (subR & controlBit) !=0 ) ledR++;
        if( (subG & controlBit) !=0 ) ledG++;
        if( (subB & controlBit) !=0 ) ledB++;

        controlBit=controlBit<<1;

        strip.setLedColorData(i, ledR, ledG, ledB);        
    } // for i

    strip.show();    

    R.step();
    G.step();
    B.step();
    
    /* Keep a watch on how much memory is used by the measurement thread */
    data.lightHighWaterMark= uxTaskGetStackHighWaterMark(NULL);
    data.lightAlive++;
    
    vTaskDelay(100);
  }; // while true
}; // taskLight

#endif // LIGHT_H
