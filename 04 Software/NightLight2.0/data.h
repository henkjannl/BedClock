#ifndef DATA_H
#define DATA_H

#include <Arduino.h>
#include "esp_system.h"

enum mainState_t      {msClock=0, msMenu};
enum powerState_t     {psTimer=0, psOff};
enum lightColor_t     {lcWhite=0, lcYellow, lcOrange, lcRed};
enum lightIntensity_t {li25=0, li35, li50, li70, li100};
enum timerDuration_t  {td03=0, td05, td10, td20, tdOff};

class tData {
  public:
    mainState_t      mainState;
    powerState_t     powerState;
    lightColor_t     lightColor;
    lightIntensity_t lightIntensity;
    timerDuration_t  timerDuration;

    bool switchButtonPressed;
    bool nextButtonPressed;
    bool selectButtonPressed;
    
    tData() {
      mainState       = msClock;
      powerState      = psTimer;
      lightColor      = lcWhite;
      lightIntensity  = li25;
      timerDuration   = td03;

      switchButtonPressed=false;
      nextButtonPressed=false;
      selectButtonPressed=false;
    };
};

/*
 * GLOBAL VARIABLES
 */

tData data;
portMUX_TYPE dataAccessMux = portMUX_INITIALIZER_UNLOCKED;

#endif // DATA_H
