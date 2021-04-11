#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>
#include "Freenove_WS2812_Lib_for_ESP32.h"

/**************************************************************************
 DEFINES AND CONSTANTS
**************************************************************************/
const uint16_t MAX_INTENSITY   = 255;

const color_t COLORS[]  = { { 1.000, 1.000, 1.000 },   // White
                            { 1.000, 1.000, 0.500 },   // Yellow
                            { 1.000, 0.750, 0.500 },   // Orange
                            { 1.000, 0.500, 0.500 } }; // Red

const float INTENSITIES[] = {0.25, 0.35, 0.50, 0.70, 1.00};

/**************************************************************************
TYPES, CLASSES, ENUMS
**************************************************************************/
class ControlVariable {
    public:
        ControlVariable() { target=0; previous=0; progress=0; step=0.02; };
        void  setTarget(float value) { previous=target; target=value; progress=0; };
        void  setStep(float value) { step=value; };
        float getCurrent() {return (previous+progress*(target-previous)); };
        float getTarget() {return target; };
        void  scan() { if (progress<1) { progress=min(progress+step, (float) 1.0); }; };

    protected:
        float previous, target;
        float progress, step;
};

class LEDs {
    public:
        LEDs(int count=16, int pin=23, int channel=0, LED_TYPE type=TYPE_GRB);

        void setPower(float x)       { power.setTarget(x)};     };
        void setIntensity(float x);  { intensity.setTarget(x)}; };
        void setRed(float x);        { red.setTarget(x)};       };
        void setGreen(float x);      { green.setTarget(x)};     };
        void setBlue(float x);       { blue.setTarget(x)};      };

        void scan();

    protected:
        Freenove_ESP32_WS2812 strip;

        ControlVariable power;
        ControlVariable intensity;
        ControlVariable red;
        ControlVariable green;
        ControlVariable blue;
};

#endif // LEDS_H