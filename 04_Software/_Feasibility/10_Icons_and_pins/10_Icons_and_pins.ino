#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <list>
#include "Icons.h"
#include "Menu.h"
#include "light.h"


// Initilize OLED 
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R2, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  

// Light
Light light(16, 23, 0, TYPE_GRB); 

// Menu structure
tMenu menu;

// Communication between interrupt routine and main program
volatile bool leftPressed, topPressed, rightPressed;


// Interrupt handling
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer(){
  static volatile bool l[3], t[3], r[3];

  portENTER_CRITICAL_ISR(&timerMux);

  // Left pin
  l[2]=l[1];
  l[1]=l[0];
  l[0]=(touchRead(4)<50);
  if (!l[2] && l[1] && l[0]) leftPressed=true;

  // Right pin
  r[2]=r[1];
  r[1]=r[0];
  r[0]=(touchRead(12)<50);
  if (!r[2] && r[1] && r[0]) rightPressed=true;
  
  // Top pin
  t[2]=t[1];
  t[1]=t[0];
  t[0]=(touchRead(14)<50);
  if (!t[2] && t[1] && t[0]) topPressed=true;

  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup(void) {
  Serial.begin(115200);

  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  timer = timerBegin(0, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);

  // Set alarm to call onTimer function every millisecond (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer, 10000, true);

  // Start an alarm
  timerAlarmEnable(timer);

  u8g2.begin();
}

void loop(void) {

  int x;
  bool l,r,t;

  // Copy the data from the interrupt routine
  portENTER_CRITICAL(&timerMux);
  l=leftPressed;  leftPressed=false;
  r=rightPressed; rightPressed=false;
  t=topPressed;   topPressed=false;
  portEXIT_CRITICAL(&timerMux);

  
  if(l) {
    menu.IncMain();
    light.showMenu();
  }

  if(r) {
    menu.IncSub();
    light.showMenu();

    switch(menu.mainSelection) {
      
      case 0: // Timer
      
        switch(menu.timerSelection) {
          case 0: // 3 minutes
            light.setTimer(td03);
            Serial.println("Timer 3 min");
          break;
          
          case 1: // 5 minutes
            light.setTimer(td05);
            Serial.println("Timer 5 min");
          break;
          
          case 2: // 10 minutes
            light.setTimer(td10);
            Serial.println("Timer 10 min");
          break;
          
          case 3: // 20 minutes
            light.setTimer(td20);
            Serial.println("Timer 20 min");
          break;          
        }
        
      light.on();
      break;

      case 1: // Intensity
        switch(menu.intensitySelection) {
          case 0: // Intensity 25%
            light.setIntensity(li25);
            Serial.println("Light intensity 25%");
          break;

          case 1: // Intensity 35%
            light.setIntensity(li35);
            Serial.println("Light intensity 35%");
          break;

          case 2: // Intensity 50%
            light.setIntensity(li50);
            Serial.println("Light intensity 50%");
          break;

          case 3: // Intensity 70%
            light.setIntensity(li70);
            Serial.println("Light intensity 70%");
          break;

          case 4: // Intensity 100%
            light.setIntensity(li100);
            Serial.println("Light intensity 100%");
          break;

        }
        
      light.on();
      break;

      case 2: // Color
        switch(menu.colorSelection) {
          case 0: // Color White
            light.setColor(lcWhite);
            Serial.println("Color white");
          break;

          case 1: // Color Yellow
            light.setColor(lcYellow);
            Serial.println("Color yellow");
          break;

          case 2: // Color Orange
            light.setColor(lcOrange);
            Serial.println("Color orange");
          break;

          case 3: // Color Red
            light.setColor(lcRed);
            Serial.println("Color red");
          break;

        }

      light.on();
      break;

      case 3: // Exit
        light.showClock();
      break;

    }
  }

  if(t) {
    light.toggle();
    Serial.println( light.getLightOn() ? "Light on" : "Light off");
  }
      
  u8g2.clearBuffer();
  u8g2.setClipWindow(0, 0, 128, 32);
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(0, 0, 128, 32, background);

  u8g2.setClipWindow(0, 6, 60, 26);
 
  u8g2.setDrawColor(1);
  x= -icon_pitch+1;
  for(auto& mainIcon : menu.main) {
    u8g2.drawXBMP(x+menu.mainOffset, 6, icon_width, icon_height, icons[mainIcon]);
    x+=icon_pitch;
  }
  
  u8g2.setDrawColor(2);
  u8g2.drawBox(0,0,22,32);

  menu.mainOffset-=4; if(menu.mainOffset<0) menu.mainOffset=0;

  u8g2.setClipWindow(68, 6, 128, 26);
  u8g2.setDrawColor(1);
  x=(int) 106+icon_pitch;
  for(auto& subIcon : menu.sub) {
    u8g2.drawXBMP(x-menu.subOffset, 6, icon_width, icon_height, icons[subIcon]);
    x-=icon_pitch;
  }
  menu.subOffset-=4; if(menu.subOffset<0) menu.subOffset=0;

  u8g2.setDrawColor(2);
  u8g2.drawBox(105,6,22,20);
  
  u8g2.sendBuffer();

  light.scan();
}
