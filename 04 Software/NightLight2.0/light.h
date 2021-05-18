#ifndef LIGHT_H
#define LIGHT_H

#if CONFIG_FREERTOS_UNICORE
  #define ARDUINO_RUNNING_CORE 0
#else
  #define ARDUINO_RUNNING_CORE 1
#endif

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
const float INTENSITIES[] = {0.25, 0.35, 0.50, 0.70, 1.00};


const uint16_t SUB_BRIGHTNESS[] = {0x0000, 0x0008, 0x0208, 0x020A, 0x0A0A, 0x2A0A, 0x2A8A, 0x2AAA, 0xAAAA, 
    0xAABA, 0xABBA, 0xABBE, 0xEBBE, 0xEBBF, 0xFBBF, 0xFBFF };


// This class is used to control the values for the RGB channels of the light
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
      if( abs(target-_target)>0.001) { // New target is different from previous target
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
};

// ======== GLOBAL VARIABLES ============= 
#define LED_COUNT   16
#define LED_PIN     23
#define LED_CHANNEL  0

Freenove_ESP32_WS2812 strip;
tFloatVariable R,G,B;

void taskLight(void * parameter );

void setupLight() {
  strip = Freenove_ESP32_WS2812(LED_COUNT, LED_PIN, LED_CHANNEL, TYPE_GRB);
  strip.begin();
  strip.setBrightness(255);  

  xTaskCreatePinnedToCore(
    taskLight,              // The function containing the task
    "TaskLight",            // Name for mortals
    12000,                  // Stack size 
    NULL,                   // Parameters
    1,                      // Priority, 0=lowest, 3=highest
    NULL,                   // Task handle
    ARDUINO_RUNNING_CORE);  // Core to run the task on

}

void taskLight(void * parameter ){

  while(true) {
    
    portENTER_CRITICAL(&dataAccessMux);
    
    if(data.powerState==psOff) {
      R.setTarget(0);
      G.setTarget(0);
      B.setTarget(0);
    }
    else {
      R.setTarget(MAX_INTENSITY*COLORS[data.lightColor].R*INTENSITIES[data.lightIntensity]);
      G.setTarget(MAX_INTENSITY*COLORS[data.lightColor].G*INTENSITIES[data.lightIntensity]);
      B.setTarget(MAX_INTENSITY*COLORS[data.lightColor].B*INTENSITIES[data.lightIntensity]);
    }

    /* Keep a watch on how much memory is used by the measurement thread */
    data.lightHighWaterMark= uxTaskGetStackHighWaterMark(NULL);
    data.lightAlive++;

    portEXIT_CRITICAL(&dataAccessMux);
    
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
    }

    strip.show();    

    R.step();
    G.step();
    B.step();
    
    vTaskDelay(100);
  };
};

#endif // LIGHT_H
