#pragma once

#include <Wire.h>
#include <U8g2lib.h>
#include "b_Icons.h"

#define ADVICE_FONT u8g2_font_7x14B_tf    // Quite good

#define WEATHER_FONT u8g2_font_helvB14_tf

// ======== GLOBAL VARIABLES ============= 
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R2, 22, 21, U8X8_PIN_NONE);  

void setScreenBrightness(screenBrightness_t brightness) {
  /* Typical values for the parameters:
   *    setScreenBrightness(255, 0,  1, 1); // most bright
   *    setScreenBrightness( 16, 0,  1, 1);
   *    setScreenBrightness(  5, 0,  0, 0);
   *    setScreenBrightness(  3, 0,  0, 0);
   *    setScreenBrightness(  1, 0,  0, 0); // least bright   */
  uint8_t contrast, vcom, p1, p2;

  switch( brightness ) {
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
}

int widthOfAdviceString( String s ) {
  u8g2.setFont( ADVICE_FONT );  
  return u8g2.getUTF8Width( s.c_str() ); 
}

bool adviceFitsOnScreen( String s ) {
  u8g2.setFont( ADVICE_FONT );  
  return ( u8g2.getUTF8Width( s.c_str() ) <= u8g2.getDisplayWidth() ); 
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
  float x;
  float dx;
  int16_t y;
  
  Serial.print("loopDisplay > ");
  
  u8g2.clearBuffer();

  switch( data.screen ) {
    
    case scnMain:
      Serial.println("scnMain");
      //u8g2.setFont(u8g2_font_helvR10_tf); // _tf includes degree symbol
      u8g2.setFont(u8g2_font_courR18_tf);      

      u8g2.setFontPosCenter();
      u8g2.setCursor( ( (u8g2.getDisplayWidth() - 32 - (u8g2.getUTF8Width( data.displayClock ))) / 2) , 16);
      u8g2.print( data.displayClock );        

      if( WEATHER_ICONS.count( data.weatherIcon ) == 1 ) {        
        u8g2.drawXBMP( 96, 0, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, *WEATHER_ICONS[ data.weatherIcon ]  );
      }
      else {
        Serial.printf("Could not find weather icon %s\n", data.weatherIcon);
      }
      
    break; // case scnMain

    case scnWeather:
      Serial.println("scnWeather");

      u8g2.setFont(WEATHER_FONT);  
      u8g2.setFontPosCenter();
      u8g2.setCursor( 2 , 16);
      Serial.printf("Wheather displaying temperature %s\n", data.displayTemperature );
      u8g2.print( data.displayTemperature );    

      x  = 2 + u8g2.getUTF8Width( data.displayTemperature ) + 3;
      
      if( data.precipitationExpected ) {
  
        // Display bar graph with precipitation
        dx = ( u8g2.getDisplayWidth() - x ) / data.precipitation.size();
        y=u8g2.getDisplayHeight()-2;

        int nextHour = int(time(NULL)/3600+1) * 3600;
  
        for (precipitation_t & p : data.precipitation) {
          int h=(int) 14.4*log(p.prec+1); // Curve through (0 , 0) (0.25 , 3.2) (3 , 20.0)
          if( h>u8g2.getDisplayHeight()-4 ) h = u8g2.getDisplayHeight()-4;
          Serial.printf("x %d prec: %.1f y-h: %d h: %d\n" , (int) x, p.prec, y-h, h );
          for( int i = 0; i<dx; i++ ) u8g2.drawVLine( (int16_t)  x+1 , y-h, h );

          if( p.t>=nextHour ) {
            u8g2.drawVLine( (u8g2_uint_t) x, (u8g2_uint_t) 0,                         (u8g2_uint_t)                         3 );
            u8g2.drawVLine( (u8g2_uint_t) x, (u8g2_uint_t) u8g2.getDisplayHeight()-4, (u8g2_uint_t) u8g2.getDisplayHeight()-1 );
            nextHour+=3600;
          }
          x+=dx;
        }
      }
      else {
        // Display no rain icon 
        u8g2.drawXBMP((int16_t) x, 0, NoRain_width, NoRain_height, NoRain_bits );
      }

    break; // case scnWeather

    case scnAdvice:
      Serial.println("scnAdvice");
      u8g2.setFont( ADVICE_FONT );  
      u8g2.setFontPosCenter();

      switch( data.displayAdviceLines ) {
        case 0:
          Serial.println( "Want to display advice but data.displayAdviceLines == 0");
        break;

        case 1:
          u8g2.setCursor( ( (u8g2.getDisplayWidth() - (u8g2.getUTF8Width( data.displayAdvice[0] ))) / 2) , 16);
          u8g2.print( data.displayAdvice[0] );    
        break;

        case 2:
          u8g2.setCursor( ( (u8g2.getDisplayWidth() - (u8g2.getUTF8Width( data.displayAdvice[0] ))) / 2) , 8);
          u8g2.print( data.displayAdvice[0] );    
          u8g2.setCursor( ( (u8g2.getDisplayWidth() - (u8g2.getUTF8Width( data.displayAdvice[1] ))) / 2) , 24);
          u8g2.print( data.displayAdvice[1] );    
        break;

        default:
          Serial.println( "Want to display advice but data.displayAdviceLines invalid");
      } // switch( data.displayAdviceLines )

      data.newAdviceRequested = true;
    break; // case data.screen == scnAdvice
    
  } // switch( data.screen )

  u8g2.sendBuffer();

  data.updateScreen = false;
}
