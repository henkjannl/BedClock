#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <list>
#include <string>

#include "gui.h"
#include "display.h"

using namespace std;

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R2, 22, 21, U8X8_PIN_NONE);  
tDisplay display(u8g2);

void setup(void) {
  Serial.begin(115200);
  u8g2.initDisplay();
  delay(500);
  u8g2.begin();
}

void loop(void) {

  if(display.wnMain.ready()) {
    if     (display.sequence==tsMainWindow)    { display.mainWindow();    display.sequence=tsGotoTopRow;    Serial.println("tsGotoTopRow"); }
    else if(display.sequence==tsGotoTopRow)    { display.showTopRow();    display.sequence=tsGotoSecondRow; Serial.println("tsGotoSecondRow"); }
    else if(display.sequence==tsGotoSecondRow) { display.showSecondRow(); display.sequence=tsMainWindow;    Serial.println("tsMainWindow"); };
    delay(800);
  } // wnMain ready

  u8g2.clearBuffer();  
  display.display(u8g2);
  u8g2.sendBuffer();

  display.step();
}
