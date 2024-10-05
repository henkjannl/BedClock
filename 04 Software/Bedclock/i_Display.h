#pragma once

#include <Wire.h>
#include <U8g2lib.h>
#include <list>

#include "b_Icons.h"        // Icons to show on screen

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

  static std::list< String > unexpectedIcons;
  bool iconPresent, iconFound;

  // Check if weather was last retrieved less than an hour ago
  bool weatherFresh = ( difftime( time( NULL ), data.lastWeatherUpdate ) < 3600.0 ); 
  
  Serial.print("loopDisplay > ");
  
  u8g2.clearBuffer();

  switch( data.screen ) {
    
    case scnMain:
      Serial.println("scnMain");
      //u8g2.setFont(u8g2_font_helvR10_tf); // _tf includes degree symbol
      u8g2.setFont(u8g2_font_courR18_tf);      

      u8g2.setFontPosCenter();

      iconPresent = ( WEATHER_ICONS.count( data.weatherIcon ) > 0 );
      
      if( weatherFresh && iconPresent ) {
        u8g2.setCursor( ( (u8g2.getDisplayWidth() - 32 - (u8g2.getUTF8Width( data.displayClock ))) / 2) , 20);
        u8g2.print( data.displayClock );        
        u8g2.drawXBMP( 96, 0, WEATHER_ICON_WIDTH, WEATHER_ICON_HEIGHT, *WEATHER_ICONS[ data.weatherIcon ]  );
      }  
      else {
        u8g2.setCursor( ( (u8g2.getDisplayWidth()      - (u8g2.getUTF8Width( data.displayClock ))) / 2) , 20);
        u8g2.print( data.displayClock );        
      } // if( weatherFresh && iconPresent )

      if( !iconPresent ) {
        // See if this icon is in the list of unexpected icons
        iconFound = false;
        for( auto & iconInList : unexpectedIcons ) if( iconInList == String( data.weatherIcon ) ) iconFound = true;
  
        if( !iconFound ) {
          unexpectedIcons.push_back( String( data.weatherIcon ) );
          sendMessageToKeyUser( String( "The following weather icon is not supported: [" ) + data.weatherIcon + "]" );
        }
      } // iconPresent
      
    break; // case scnMain

    case scnWeather1:
      Serial.println("scnWeather1");

      if( data.weatherUpdated ) {
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
          u8g2.setFont( u8g2_font_7x14_tf );  
          u8g2.setFontPosCenter();
          u8g2.setCursor( 50,  8 );
          u8g2.print( "NO PRECIP." );    
          u8g2.setCursor( 50, 24 );
          u8g2.print( "EXPECTED" );    
          // Display no rain icon 
          //u8g2.drawXBMP( u8g2.getDisplayWidth() - NoRain_width , 0, NoRain_width, NoRain_height, NoRain_bits );          
        }
      } else {
          u8g2.setFont( u8g2_font_7x14_tf );  
          u8g2.setFontPosCenter();
          u8g2.setCursor( 5,  8 );
          u8g2.print( "WEATHER DATA" );    
          u8g2.setCursor( 5, 24 );
          u8g2.print( "NOT AVAILABLE" );    
      }

    break; // case scnWeather1

    case scnWeather2:
      Serial.println("scnWeather2");

      if( weatherFresh ) {
        const char* TEMP_LABEL = "outside";
        const char* MAX_DAY_LABEL = "max day";
        const char* HUMID_LABEL = "humidity";

        #define WEATHER2_LABEL u8g2_font_simple1_tf
        #define WEATHER2_VALUE u8g2_font_crox4hb_tf
        #define WEATHER2_UNIT  u8g2_font_crox1hb_tf
        
        u8g2.setFont( WEATHER2_LABEL );
        int labelSpace = ( u8g2.getDisplayWidth() - ( u8g2.getUTF8Width( TEMP_LABEL ) + u8g2.getUTF8Width( MAX_DAY_LABEL ) + u8g2.getUTF8Width( HUMID_LABEL ) ) ) / 2;
        int maxDayTempX  = u8g2.getUTF8Width( TEMP_LABEL ) + labelSpace;
        int humidX = u8g2.getDisplayWidth() - u8g2.getUTF8Width( HUMID_LABEL );
  
        u8g2.setFontPosTop();
        u8g2.setCursor( 0           , 0);  u8g2.print( TEMP_LABEL    );
        u8g2.setCursor( maxDayTempX , 0);  u8g2.print( MAX_DAY_LABEL );
        u8g2.setCursor( humidX      , 0);  u8g2.print( HUMID_LABEL   );
  
        int w[6];
        char outTemp[5];    snprintf( outTemp,    sizeof( outTemp    ), "%d", (int) data.outsideTemp    );
        char maxDayTemp[5]; snprintf( maxDayTemp, sizeof( maxDayTemp ), "%d",       data.maxDisplayTemp );
        char humid[5];      snprintf( humid,      sizeof( humid      ), "%d",       data.humidity       );
        
        u8g2.setFont( WEATHER2_VALUE );
        w[0] = u8g2.getUTF8Width( outTemp );
        w[2] = u8g2.getUTF8Width( maxDayTemp );
        w[4] = u8g2.getUTF8Width( humid );
  
        u8g2.setFont( WEATHER2_UNIT );
        w[1] = u8g2.getUTF8Width( DEG_C );
        w[3] = u8g2.getUTF8Width( DEG_C );
        w[5] = u8g2.getUTF8Width(  "%"  );
        labelSpace = ( u8g2.getDisplayWidth() - ( w[0] + 1 + w[1] +w[2] + 1 + w[3] + w[4] + 1 + w[5] ) ) / 2;
  
        int x[6];
        x[0] = 0;
        x[1] = x[0] + w[0] + 1;
        x[2] = x[1] + w[1] + labelSpace;
        x[3] = x[2] + w[2] + 1;
        x[4] = x[3] + w[3] + labelSpace;
        x[5] = x[4] + w[4] + 1;
          
        u8g2.setFont( WEATHER2_VALUE );
        const int y = 11; //u8g2.getDisplayHeight() - 23; // u8g2.getMaxCharHeight();
        u8g2.setCursor( x[0], y );  u8g2.print( outTemp );
        u8g2.setCursor( x[2], y );  u8g2.print( maxDayTemp );
        u8g2.setCursor( x[4], y );  u8g2.print( humid );
  
        u8g2.setFont( WEATHER2_UNIT );
        u8g2.setCursor( x[1], y );  u8g2.print( DEG_C );
        u8g2.setCursor( x[3], y );  u8g2.print( DEG_C );
        u8g2.setCursor( x[5], y );  u8g2.print(  "%"  );
      } else {
          u8g2.setFont( u8g2_font_7x14_tf );  
          u8g2.setFontPosCenter();
          u8g2.setCursor( 5,  8 );
          u8g2.print( "WEATHER DATA" );    
          u8g2.setCursor( 5, 24 );
          u8g2.print( "NOT AVAILABLE" );    
      }
      
    break; // case data.screen == scnWeather2
    
  } // switch( data.screen )

  u8g2.sendBuffer();

  data.updateScreen = false;
}
