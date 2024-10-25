#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R2, 22, 21, U8X8_PIN_NONE);  

// value from 0 to 7, higher values more brighter
void setSSD1306VcomDeselect(uint8_t v)
{  
  u8x8_cad_StartTransfer(u8g2.getU8x8());
  u8x8_cad_SendCmd(u8g2.getU8x8(), 0x0db);
  u8x8_cad_SendArg(u8g2.getU8x8(), v << 4);
  u8x8_cad_EndTransfer(u8g2.getU8x8());
}

// p1: 1..15, higher values, more darker, however almost no difference with 3 or more
// p2: 1..15, higher values, more brighter
void setSSD1306PreChargePeriod(uint8_t p1, uint8_t p2)
{ 
  u8x8_cad_StartTransfer(u8g2.getU8x8());
  u8x8_cad_SendCmd(u8g2.getU8x8(), 0x0d9);
  u8x8_cad_SendArg(u8g2.getU8x8(), (p2 << 4) | p1 );
  u8x8_cad_EndTransfer(u8g2.getU8x8());
}

void setScreenBrightness(uint8_t contrast, uint8_t vcom, uint8_t p1, uint8_t p2) {
  /* Typical values for the parameters:
   *    setScreenBrightness(255, 0,  1, 1); // most bright
   *    setScreenBrightness( 16, 0,  1, 1);
   *    setScreenBrightness(  5, 0,  0, 0);
   *    setScreenBrightness(  3, 0,  0, 0);
   *    setScreenBrightness(  1, 0,  0, 0); // least bright   */
  u8g2.setContrast(contrast);
  u8x8_cad_StartTransfer(u8g2.getU8x8());
  u8x8_cad_SendCmd(u8g2.getU8x8(), 0x0db);
  u8x8_cad_SendArg(u8g2.getU8x8(), vcom << 4);
  u8x8_cad_SendCmd(u8g2.getU8x8(), 0x0d9);
  u8x8_cad_SendArg(u8g2.getU8x8(), (p2 << 4) | p1 );
  u8x8_cad_EndTransfer(u8g2.getU8x8());
}

void setup(void) {
  u8g2.begin();
}

void changeSettings(uint8_t contrast, uint8_t vcom, uint8_t precharge1, uint8_t precharge2) {
  u8g2.clearBuffer();          // clear the internal memory
  setScreenBrightness(contrast, vcom, precharge1, precharge2);
  //setSSD1306VcomDeselect(vcom);
  //setSSD1306PreChargePeriod(precharge1, precharge2);
  //u8g2.setContrast(contrast);
  u8g2.setCursor(0,  9);  u8g2.print("contrast");   u8g2.setCursor(100,  9);  u8g2.print(contrast);
  u8g2.setCursor(0, 17);  u8g2.print("vcom");       u8g2.setCursor(100, 17);  u8g2.print(vcom);
  u8g2.setCursor(0, 25);  u8g2.print("precharge1"); u8g2.setCursor(100, 25);  u8g2.print(precharge1);
  u8g2.setCursor(0, 31);  u8g2.print("precharge2"); u8g2.setCursor(100, 31);  u8g2.print(precharge2);
  u8g2.sendBuffer();          // transfer internal memory to the display
  delay(1000);  
}
void loop(void) {
  u8g2.setFont(u8g2_font_profont10_tf); // choose a suitable font
  changeSettings(255, 0,  1, 1);
  changeSettings( 16, 0,  1, 1);
  changeSettings(  5, 0,  0, 0);
  changeSettings(  3, 0,  0, 0);
  changeSettings(  1, 0,  0, 0);
}
