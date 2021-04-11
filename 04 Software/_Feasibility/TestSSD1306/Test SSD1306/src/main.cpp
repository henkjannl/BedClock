#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

#define SDA 21
#define SCL 22
#define U8X8_HAVE_HW_I2C

//U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 21, /* data=*/ 20, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather M0 Basic Proto + FeatherWing OLED
U8G2_SSD1306_128X32_UNIVISION_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // Adafruit Feather ESP8266/32u4 Boards + FeatherWing OLED
//U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED
//U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ SCL, /* data=*/ SDA);   // pin remapping with ESP8266 HW I2C


void setup() {
  // put your setup code here, to run once:
  //delay(250);
  //Wire.begin();
  delay(250);
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x10_tr);
}

void draw(const char *s)
{
  u8g2.firstPage();
  do {
    u8g2.drawStr(2,15,"Hello World!");    
    u8g2.drawStr(2,30,s);    
    u8g2.drawFrame(0,0,u8g2.getDisplayWidth(),u8g2.getDisplayHeight() );

    u8g2.drawStr(2, 45, u8x8_u16toa(u8g2.getDisplayWidth(), 5));
    u8g2.drawStr(2, 60, u8x8_u16toa(u8g2.getDisplayHeight(), 5));
    
  } while ( u8g2.nextPage() );
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  u8g2.clearDisplay();
  u8g2.setDisplayRotation(U8G2_R0);
  u8g2.clearDisplay();
  u8g2.setFlipMode(1);
  draw("R0,F1");

}