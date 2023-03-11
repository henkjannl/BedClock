#pragma once

#include "a_Data.h"
#include <Freenove_WS2812_Lib_for_ESP32.h>
#include <ArduinoJson.h>
#include <vector>

// ======== GLOBAL VARIABLES ============= 
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LED_COUNT, LED_PIN, LED_CHANNEL, TYPE_GRB);

void setupLight() {
  // Initialize light
  Serial.println( "Initialize light" );
  strip.begin();
  strip.setBrightness(255);    
}

void loopLight() {
    
  // Get the integer component
  uint16_t bR=(uint16_t) data.R.getValue();
  uint16_t bG=(uint16_t) data.G.getValue();
  uint16_t bB=(uint16_t) data.B.getValue();

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

  data.R.step();
  data.G.step();
  data.B.step();
};
