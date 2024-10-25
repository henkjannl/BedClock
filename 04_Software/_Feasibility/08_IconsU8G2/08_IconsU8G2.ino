#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <list>
#include "Icons.h"

const int icon_width  = 20;
const int icon_height = 20;
const int icon_pitch  = icon_width+2;

#define MAX_ICONS_PER_MENU 8


// Initilize OLED 
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R2, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   

const byte mainIcons[]      = { 0, 1, 2, 3};    const byte mainCount      =4;
const byte timerIcons[]     = { 4, 5, 6, 7};    const byte timerCount     =4;
const byte intensityIcons[] = { 8, 9,10,11,12}; const byte intensityCount =5;
const byte colorIcons[]     = {13,14,15,16};    const byte colorCount     =4;
const byte exitIcons[]      = {17,18};          const byte exitCount      =2;
const byte selectOffset=17;

struct tMenu {
  byte mainSelection;
  byte intensitySelection;
  byte timerSelection;
  byte colorSelection;
  byte exitSelection;

  std::list<byte> main;
  std::list<byte> sub;

  int mainOffset=0;
  int subOffset=0;

  tMenu() {
    mainSelection=0;
    
    intensitySelection=0;
    timerSelection=0;
    colorSelection=0;
    exitSelection=0;

    tIcons();
  }
 
  void tIcons() {
    
    main.clear();
    for(int i=0; i<4; i++) {
      byte b=mainIcons[(mainSelection+i-1+mainCount) % mainCount]; 
      main.push_back(b);
    }

    // Add icon numbers to the submenu
    sub.clear();
    switch(mainSelection) {
      case 0: // Intensity is selected
        for(int i=0; i<4; i++) {
          byte b=intensityIcons[(intensitySelection+i-1+intensityCount) % intensityCount]; 
          sub.push_back(b);
        }
        break;
        
      case 1: // Timer is selected
        for(int i=0; i<4; i++) {
          byte b=timerIcons[(timerSelection+i-1+timerCount) % timerCount]; 
          sub.push_back(b);
        }
        break;

      case 2: // Color is selected
        for(int i=0; i<4; i++) {
          byte b=colorIcons[(colorSelection+i-1+colorCount) % colorCount];
          sub.push_back(b);
        }
        break;

      case 3: // Exit is selected
        for(int i=0; i<3; i++) {
          byte b=exitIcons[(exitSelection+i-1+exitCount) % exitCount];
          sub.push_back(b);
        }
        break;
    }
  }
  
  void IncMain() {
    mainSelection=(mainSelection+1) % mainCount;
    
    tIcons();
    mainOffset=icon_pitch;
    
    Serial.printf("Main selection: %d\n", mainSelection);    
    
    for(auto& ic : main) {
      Serial.print(ic);
      Serial.print(" > ");
    }
    Serial.println(".");    
    
    for(auto& ic : sub) {
      Serial.print(ic);
      Serial.print(" > ");
    }
    Serial.println(".");    

  }
  
  void IncSub() {
    subOffset=icon_pitch;
    switch(mainSelection) {
      case 0: // Intensity is selected
        intensitySelection=(intensitySelection+1) % intensityCount;
        break;
        
      case 1: // Timer is selected
        timerSelection=(timerSelection+1)%timerCount;
        break;

      case 2: // Color is selected
        colorSelection=(colorSelection+1)%colorCount;
        break;

      case 3: // Exit is selected
        exitSelection=(exitSelection+1)%exitCount;
        break;
    }
    tIcons();
  }
};

// Global variables
tMenu menu;


void setup(void) {
  Serial.begin(115200);
  u8g2.begin();
}

void loop(void) {
  static unsigned long nextTime=0;
  int x;
  
  if(millis()>nextTime) {
    static int state=0;
    if((state++ % 5)==0)
      menu.IncMain();
    else
      menu.IncSub(); 
    nextTime=millis()+2000;  
  }
    
  u8g2.clearBuffer();
  u8g2.setClipWindow(0, 0, 128, 32);
  u8g2.setDrawColor(1);
  u8g2.drawXBMP(0, 0, 128, 32, background);

  u8g2.setClipWindow(0, 6, 60, 26);
 
  u8g2.setDrawColor(1);
  x= -icon_pitch+1;
  Serial.printf("First X: %d\n" , x);
  for(auto& mainIcon : menu.main) {
    u8g2.drawXBMP(x+menu.mainOffset, 6, icon_width, icon_height, icons[mainIcon]);
    Serial.printf("X: %d M: %d >" , x,menu.mainOffset);
    x+=icon_pitch;
  }
  Serial.println();
  
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
  //delay(40);

  

}
