#pragma once

#include <Wire.h>
#include <U8g2lib.h>

//#define ADVICE_FONT u8g2_font_courR10_tf
//#define ADVICE_FONT u8g2_font_helvR08_tf
//#define ADVICE_FONT u8g2_font_helvB10_tf
//#define ADVICE_FONT u8g2_font_helvB08_tf
//#define ADVICE_FONT u8g2_font_Engrish_tf
//#define ADVICE_FONT u8g2_font_koleeko_tf 
//#define ADVICE_FONT u8g2_font_7x14_tf 
#define ADVICE_FONT u8g2_font_7x14B_tf    // Quite good
//#define ADVICE_FONT u8g2_font_lubI08_tf
//#define ADVICE_FONT u8g2_font_BBSesque_tf
//#define ADVICE_FONT u8g2_font_7x14_tf
//#define ADVICE_FONT u8g2_font_profont12_tf // too small

// No rain icon
#define NoRain_width 32
#define NoRain_height 32
static unsigned char NoRain_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x60, 0xfe, 0x03, 0x00,
   0xf0, 0x0e, 0x0f, 0x00, 0xe0, 0x01, 0x1c, 0x00, 0xc0, 0x03, 0xf8, 0x03,
   0x80, 0x07, 0xf8, 0x07, 0x00, 0x0f, 0x00, 0x0e, 0x40, 0x1e, 0x00, 0x0c,
   0xc0, 0x3c, 0x00, 0x0c, 0xf8, 0x79, 0x00, 0x1c, 0x3c, 0xf0, 0x00, 0x3c,
   0x0e, 0xe0, 0x01, 0x70, 0x07, 0xc0, 0x03, 0x60, 0x03, 0x80, 0x07, 0xc0,
   0x03, 0x00, 0x0f, 0xc0, 0x03, 0x00, 0x1e, 0xc0, 0x03, 0x00, 0x3c, 0xc0,
   0x06, 0x00, 0x78, 0x60, 0x0e, 0x00, 0xf0, 0x70, 0xfc, 0xff, 0xe7, 0x39,
   0xf0, 0xff, 0xcf, 0x1b, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x0f,
   0x10, 0x20, 0x40, 0x1e, 0x18, 0x30, 0x60, 0x3c, 0x38, 0x70, 0x60, 0x18,
   0x3c, 0x78, 0xf0, 0x00, 0x3c, 0x78, 0xf0, 0x00, 0x3c, 0x78, 0xf0, 0x00,
   0x38, 0x70, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00 };

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
      u8g2.setCursor( ( (u8g2.getDisplayWidth() - (u8g2.getUTF8Width( data.displayClock ))) / 2) , 16);
      u8g2.print( data.displayClock );        
    break; // case scnMain

    case scnWeather:
      Serial.println("scnWeather");

      u8g2.setFont(u8g2_font_courR12_tf);  
      u8g2.setFontPosCenter();
      u8g2.setCursor( 2 , 16);
      Serial.printf("Wheather displaying temperature %s\n", data.displayTemperature );
      u8g2.print( data.displayTemperature );    

      x  = 2 + u8g2.getUTF8Width( data.displayTemperature ) + 5;
      
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
        u8g2.drawXBMP(64,0, NoRain_width, NoRain_height, NoRain_bits );
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
