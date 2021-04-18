#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <list>
#include "Icons.h"
#include "Menu.h"


// Initilize OLED 
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R2, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   
volatile bool leftPressed, topPressed, rightPressed;

// Menu structure
tMenu menu;

// Interrupt handling
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer(){
  static volatile bool l[3], t[3], r[3];

  portENTER_CRITICAL_ISR(&timerMux);

  l[2]=l[1];
  l[1]=l[0];
  l[0]=(touchRead(4)<50);
  if (!l[2] && l[1] && l[0]) leftPressed=true;

  r[2]=r[1];
  r[1]=r[0];
  r[0]=(touchRead(12)<50);
  if (!r[2] && r[1] && r[0]) rightPressed=true;
  
  t[2]=t[1];
  t[1]=t[0];
  t[0]=(touchRead(14)<50);
  if (!t[2] && t[1] && t[0]) topPressed=true;

  cntr++;
  
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

  portENTER_CRITICAL(&timerMux);
  l=leftPressed;  leftPressed=false;
  r=rightPressed; rightPressed=false;
  t=topPressed;   topPressed=false;
  portEXIT_CRITICAL(&timerMux);

  Serial.println(cntr, l);
  
  if(l) {
    menu.IncMain();
    Serial.println("Left button pressed");
  }

  if(r) {
    menu.IncSub();
    Serial.println("Right button pressed");
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
  
  delay(40);
}
