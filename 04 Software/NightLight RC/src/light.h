#ifndef LIGHT_H
#define LIGHT_H

#include <Arduino.h>

/**************************************************************************
 DEFINES AND CONSTANTS
**************************************************************************/
const int   TIME_INTERVAL      = 10;       // Update interval for light intensity
const int   MENU_TIMEOUT       = 20*1000;  // Switch to the clock 
const float PROGRESS_INCREMENT = 0.02;

typedef struct { float R,G,B; } color_t;

const color_t COLORS[]  = { { 1.000, 1.000, 1.000 },   // White
                            { 1.000, 1.000, 0.500 },   // Yellow
                            { 1.000, 0.750, 0.500 },   // Orange
                            { 1.000, 0.500, 0.500 } }; // Red

const float INTENSITIES[] = {0.25, 0.35, 0.50, 0.70, 1.00};

/**************************************************************************
TYPES, CLASSES, ENUMS
**************************************************************************/
enum mainState_t      {msClock=0, msMenu};
enum powerState_t     {psTimer=0, psOff};
enum lightColor_t     {lcWhite=0, lcYellow, lcOrange, lcRed};
enum lightIntensity_t {li25=0, li35, li50, li70, li100};
enum timerDuration_t  {td03=0, td05, td10, td20, tdOff};

class Light {
    public:
        Light();

        void toggle();
        void on();
        void off()   { power.setTarget(0); };
        void scan();
        void showClock() { mainState=msClock; };
        void showMenu()  { mainState=msMenu; menuTimeOut=millis()+MENU_TIMEOUT; };
        bool menuSwitchedOn() { return (mainState==msMenu); };

        char debug[100];

        void setTimer(timerDuration_t duration);
        void setIntensity(lightIntensity_t value) { intensity.setTarget(INTENSITIES[value]); };
        void setColor(lightColor_t color);

        float getIntensity() { return intensity.getTarget(); };

    protected:
        LEDs leds(count=16, pin=23, channel=0, type=TYPE_GRB);

        unsigned long timer, menuTimeOut;

        mainState_t      mainState=msClock;
        powerState_t     powerState=psTimer;
        timerDuration_t  timerDuration=td03;

};

#endif // LIGHT_H