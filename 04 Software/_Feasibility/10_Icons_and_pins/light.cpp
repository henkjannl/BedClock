#include "light.h"

#include <cmath>

const uint16_t SUB_BRIGHTNESS[] = {0x0000, 0x0008, 0x0208, 0x020A, 0x0A0A, 0x2A0A, 0x2A8A, 0x2AAA, 0xAAAA, 
    0xAABA, 0xABBA, 0xABBE, 0xEBBE, 0xEBBF, 0xFBBF, 0xFBFF };

Light::Light(int count, int pin, int channel, LED_TYPE type) {
  pinMode(pin, OUTPUT);
  strip = Freenove_ESP32_WS2812(count, pin, channel, type);
  strip.begin();
  strip.setBrightness(255);  

  setTimer(td03);
  setIntensity(li25);
  setColor(lcWhite);

  on();
  showMenu();
}

void Light::on() {
    power.setTarget(1);
    
    switch (timerDuration) {
        case td03:  timer=millis()+ 3*60*1000;  break;
        case td05:  timer=millis()+ 5*60*1000;  break;
        case td10:  timer=millis()+10*60*1000;  break;
        case td20:  timer=millis()+20*60*1000;  break;
        default:                                break;
    }
}

void Light::toggle() {
    if (powerState==psTimer) {
        powerState=psOff;
        off();
    }
    else {
        powerState=psTimer;
        on();
    }
}

void Light::setTimer(timerDuration_t duration) { 
    timerDuration=duration; 

    // Set the new timer interval also for the current situation
    if (powerState==psTimer) on(); 
};

void Light::setColor(lightColor_t color) { 
    red  .setTarget(COLORS[color].R);
    green.setTarget(COLORS[color].G);
    blue .setTarget(COLORS[color].B);
}

int linearize(float intensity) {
  /*
  The function takes a float in the domain from 0..255 and translates it 
  to an integer output value between 0..2048

  a*exp(b*x)=y    
  a*exp(b*1)=1                            (1)
  a*exp(b*255)=16*128-1                   (2)
  
  (1)    a=1/exp(b)                       (3)
  (1+3)  1/exp(b)*exp(b*255)=16*90-1    
         exp(b*255)=(16*90-1)*exp(b)    
         ln(exp(b*255))=ln((16*90-1)*exp(b))    
         b*255=ln(16*90-1)+b    
         b*254=ln(16*90-1)    
         b=ln(16*90-1)/254    
  */
  const float b=log(16*90-1)/254;
  const float a=1/exp(b);
  
  return (int) (a*exp(b*intensity));
}

void Light::scan() {
    // Check if the timer has elapsed
    if( (timerDuration!=tdOff) && (millis()>timer) ) off();

    // Check if the menu must be switched off
    if( (menuSwitchedOn()) && (millis()>menuTimeOut) ) showClock();

    // Check if we matched the timebase
    static long previousSample;

    if(abs(millis()-previousSample)<TIME_INTERVAL) {
        // We are too early, jump back to main program
        return;
    }
    else {
        previousSample+=TIME_INTERVAL;

        // Check if clock has overflowed
        if(abs(millis()-previousSample)>2*TIME_INTERVAL) {
            previousSample=millis()+TIME_INTERVAL;
        }
    }

    // Calculate all controlled variables
    power.scan();
    intensity.scan();
    red.scan();
    green.scan();
    blue.scan();

    // Calculate linearized values to send to the LEDs
    uint16_t R=linearize(MAX_INTENSITY*power.getCurrent()*intensity.getCurrent()*  red.getCurrent());
    uint16_t G=linearize(MAX_INTENSITY*power.getCurrent()*intensity.getCurrent()*green.getCurrent());
    uint16_t B=linearize(MAX_INTENSITY*power.getCurrent()*intensity.getCurrent()* blue.getCurrent());

    // Translate into sublevels for the individual levels
    uint8_t  baseR = R>>4;  uint16_t subR = SUB_BRIGHTNESS[R & 0xF]; 
    uint8_t  baseG = G>>4;  uint16_t subG = SUB_BRIGHTNESS[G & 0xF]; 
    uint8_t  baseB = B>>4;  uint16_t subB = SUB_BRIGHTNESS[B & 0xF]; 

    uint8_t ledR, ledG, ledB;
    uint16_t controlBit=0x01;

    /*
    sprintf(debug,"R%.3f G%.3f B%.3f R%d G%d B%d\n", 
                power.getCurrent()*intensity.getCurrent()*  red.getCurrent(), 
                power.getCurrent()*intensity.getCurrent()*green.getCurrent(),
                power.getCurrent()*intensity.getCurrent()* blue.getCurrent(),
                R,G,B);
    */
   
    for(uint8_t i=0; i<16; i++) {
        ledR=baseR+(subR & controlBit)? 1 : 0;
        ledG=baseG+(subG & controlBit)? 1 : 0;
        ledB=baseB+(subB & controlBit)? 1 : 0;

        controlBit=controlBit<<1;

        /*
        if((ledR>128) || (ledG>128) || (ledB>128)) {
            sprintf(temp, "[%d] %d %d %d ", i, ledR,ledG,ledB);
            Serial.printf("%s> %d %d %d\n", temp,R,G,B);
            strcat (debug,temp);
        }

        if(ledR>250) ledR=0;
        if(ledG>250) ledG=0;
        if(ledB>250) ledB=0;
        */
        strip.setLedColorData(i, ledR, ledG, ledB);
        
    }

    strip.show();    
}
