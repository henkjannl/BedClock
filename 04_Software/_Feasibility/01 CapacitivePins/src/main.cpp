#include <Arduino.h>

const int NUM_CAP_PINS = 10;
const int CAP_PINS[] = {4,0,2,15,13,12,14,27,33,32};

// Conclusions from measurements:
//  4 =     = Touch0 = working
//  0 =     = Touch1 = working
//  2 =     = Touch2 = not working (always low)
// 15 = TDO = Touch3 = working
// 13 = TCK = Touch4 = working
// 12 = TDI = Touch5 = working
// 14 = TMS = Touch6 = working
// 27 =     = Touch7 = working
// 33 =     = Touch8 = not working (always value of  99)
// 32 =     = Touch9 = not working (always value of 140)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  static unsigned long line=0;

  if((line % 7)==0) {
      for(int i=0; i<NUM_CAP_PINS; i++) {
        Serial.printf("%03d * ", CAP_PINS[i]);
      }
      Serial.println();
    }

  // put your main code here, to run repeatedly:
  for(int i=0; i<NUM_CAP_PINS; i++) {
    Serial.printf("%03d   ", touchRead(CAP_PINS[i]));
  }
  Serial.println();

  line++;

  delay(300);
}