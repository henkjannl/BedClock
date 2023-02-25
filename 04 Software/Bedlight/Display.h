#pragma once

#include <Wire.h>
#include <U8g2lib.h>



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

void loopDisplay(bool &updateScreen) {
  switch( data.screen ) {
    
    case scnMain:
      // Update clock
      time_t rawtime;
      struct tm * timeinfo;
      static int prev_min = -1;
      static bool firstClockUpdate = true;
      
      time( &rawtime );
      timeinfo = localtime( &rawtime );
      if( ( timeinfo->tm_year > 80 ) and ( timeinfo->tm_min != prev_min ) ) {
        prev_min = timeinfo->tm_min;
        updateScreen = true; 
      };

      if( updateScreen ) {
        u8g2.clearBuffer();

        char timestr[12];
        snprintf(timestr, 12, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min );
        Serial.printf("Update time %s\n", timestr);
        
        //u8g2.setFont(u8g2_font_missingplanet_tr);
        //u8g2.setFont(u8g2_font_calibration_gothic_nbp_tr);
        //u8g2.setFont(u8g2_font_courR18_tr);
        //u8g2.setFont(u8g2_font_helvR10_tf); // Includes degree symbol
        u8g2.setFont(u8g2_font_courR18_tf);

        u8g2.setFontPosCenter();
        u8g2.setCursor( ( (u8g2.getDisplayWidth() - (u8g2.getUTF8Width(timestr))) / 2) , 16);
        u8g2.print( timestr );    
    
        u8g2.sendBuffer();
        updateScreen = false;
      }
    break; // case scnMain

    case scnWeather:

      if( updateScreen ) {
        u8g2.clearBuffer();
        //u8g2.setFont(u8g2_font_helvR14_tf); // Includes degree symbol
        u8g2.setFont(u8g2_font_courR12_tf);
  
        String temperature = String( (int) weather.outsideTemp) + DEGREE_SYMBOL + "C";
        u8g2.setFontPosCenter();
        Serial.printf("%s width: %d\n" , temperature.c_str(), u8g2.getUTF8Width( temperature.c_str() ) );
        u8g2.setCursor( 2 , 16);
        u8g2.print( temperature.c_str() );    

        // Display bar graph with precipitation
        float x  = 2 + u8g2.getUTF8Width( temperature.c_str() ) + 5;
        float dx = ( u8g2.getDisplayWidth() - x ) / weather.precipitation.size();
        int16_t y=u8g2.getDisplayHeight()-2;
      
        for (precipitation_t & p : weather.precipitation) {
          int h=(int) 14.4*log(p.prec+1); // Curve through (0 , 0) (0.25 , 3.2) (3 , 20.0)
          if( h>u8g2.getDisplayHeight()-4 ) h = u8g2.getDisplayHeight()-4;
          Serial.printf("x %d prec: %.1f y-h: %d h: %d\n" , (int) x, p.prec, y-h, h );
          u8g2.drawVLine( (int16_t)  x , y-h, h );
          u8g2.drawVLine( (int16_t) x+1, y-h, h );
          x+=dx;
        }

        u8g2.sendBuffer();
        updateScreen = false;
        
      } // updateScreen
    break; // case scnWeather
    
  } // switch( data.screen )
  
}
