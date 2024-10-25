#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <list>
#include <string>
#include <stdlib.h>     

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

  if(display.grpMain.ready()) {

    if(rand() % 10<7)
      display.nextButton();
    else
      display.selectButton();

    delay(800);
  } // grpMain ready

  u8g2.setDrawColor(0);
  u8g2.clearBuffer();  
  display.display(u8g2);
  u8g2.sendBuffer();

  display.step();
}
