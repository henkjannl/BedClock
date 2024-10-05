#pragma once

#include <Wire.h>
#include <U8g2lib.h>
#include <list>

#include "b_Icons.h"        // Icons to show on screen

#define WEATHER_FONT u8g2_font_helvB14_tf

// ======== GLOBAL VARIABLES ============= 
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R2, 22, 21, U8X8_PIN_NONE);  

void setScreenBrightness(screenBrightness_t brightness) {
  uint8_t contrast, vcom, p1, p2;
  
  switch( brightness ) {
    case sb0: contrast =   0; vcom=0; p1=0; p2=0; break;
    case sb1: contrast =   1; vcom=0; p1=0; p2=0; break;
    case sb2: contrast =   3; vcom=0; p1=0; p2=0; break;
    case sb3: contrast =   5; vcom=0; p1=0; p2=0; break;
    case sb4: contrast =  16; vcom=0; p1=1; p2=1; break;
    case sb5: contrast = 255; vcom=0; p1=1; p2=1; break;
  }
   
  u8g2.setContrast(contrast);
  u8x8_cad_StartTransfer(u8g2.getU8x8());
  u8x8_cad_SendCmd(u8g2.getU8x8(), 0x0db);
  u8x8_cad_SendArg(u8g2.getU8x8(), vcom << 4);
  u8x8_cad_SendCmd(u8g2.getU8x8(), 0x0d9);
  u8x8_cad_SendArg(u8g2.getU8x8(), (p2 << 4) | p1 );
  u8x8_cad_EndTransfer(u8g2.getU8x8());
  
  if(contrast==0) { 
    Serial.println("Clearing the screen");
    u8g2.clearBuffer();
    u8g2.sendBuffer();
    u8g2.clear(); // Just to be sure, clear screen
  };

  Serial.printf("Screen brightness was set to %d\n", contrast);
}

void resetScreen() {
  Serial.println("Reset display");
  u8g2.begin();
  setScreenBrightness(data.screenBrightness);
}

void setupDisplay() {
  Serial.println( "Initialize display" );
  u8g2.initDisplay();
  delay(500);
  u8g2.begin();
  u8g2.clearBuffer();
  //u8g2.setFont(u8g2_font_cu12_tr);    
  u8g2.setFont(u8g2_font_courR10_tf);
  u8g2.setFontPosCenter();
  u8g2.setCursor( 6, 16);
  u8g2.print( ( String("Version ") + VERSION ).c_str() );    
  u8g2.sendBuffer();
};

void loopDisplay() {
  int16_t x, y;

  Serial.println("Loop display");

  // Scroll screen to the currently displayed item

  static const int16_t nominal_pos[] = {20,40,54,66,78};
  static const int16_t desired_y_offset[] = { 0, 0, 20, 34, 46, 58};
  if (data.offset_y<desired_y_offset[data.displayed_item]) data.offset_y+=2;
  if (data.offset_y>desired_y_offset[data.displayed_item]) data.offset_y-=4;
  
  u8g2.clearBuffer();

  // Display time
  u8g2.setFont(u8g2_font_courR18_tf);      
  u8g2.setFontPosCenter();
  u8g2.setCursor( ( (u8g2.getDisplayWidth() - (u8g2.getUTF8Width( data.displayClock ))) / 2) , nominal_pos[0]-data.offset_y);
  u8g2.print( data.displayClock );        

  u8g2.setFont(u8g2_font_helvR10_tf);

  // Display color
  x=86;
  y=nominal_pos[1]-data.offset_y;
  u8g2.setCursor(3,y);
  u8g2.print( "Color" );        
  for(auto c: { lcWhite, lcYellow, lcOrange, lcRed } ) {
    if(data.color==c) u8g2.drawDisc(x, y, 5); else u8g2.drawCircle(x, y, 5);
    x+=12;
  }

  // Brightness
  x=86;
  y=nominal_pos[2]-data.offset_y;
  u8g2.setCursor(3,y);
  u8g2.print( "Brightness" );        
  for(auto b: { lb15, lb30, lb50, lb100 }) {
    if(data.brightness==b) u8g2.drawDisc(x, y, 5); else u8g2.drawCircle(x, y, 5);
    x+=12;
  }

  // Timer
  x=86;
  y=nominal_pos[3]-data.offset_y;
  u8g2.setCursor(3,y);
  u8g2.print( "Timer" );        
  for(auto t: { lt03, lt05, lt10, lt20 }) {
    if(data.timer==t) u8g2.drawDisc(x, y, 5); else u8g2.drawCircle(x, y, 5);
    x+=12;
  }

  // Backlight
  x=62;
  y=nominal_pos[4]-data.offset_y;
  u8g2.setCursor(3,y);
  u8g2.print( "Display" );        
  for(auto s: { sb0, sb1, sb2, sb3, sb4, sb5 }) {
    if(data.screenBrightness==s) u8g2.drawDisc(x, y, 5); else u8g2.drawCircle(x, y, 5);
    x+=12;
  }

  u8g2.sendBuffer();

  data.updateScreen = (data.offset_y != desired_y_offset[data.displayed_item]);
}
