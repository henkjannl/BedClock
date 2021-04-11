#include "Freenove_WS2812_Lib_for_ESP32.h"

Freenove_ESP32_WS2812 strip;

#define LED_COUNT 16
#define LED_PIN   23
#define CHANNEL    0

#define NUM_DITHER 20

const utin32_t DITHER[] = {0x00000, 0x00001, 0x00401, 0x04081, 
                           0x08421, 0x11111, 0x24491, 0x49249, 
                           0x4A529, 0x2AA55, 0x55555, 0x55557, 
                           0x55D57, 0x5D5D7, 0x5DD77, 0x77777, 
                           0x7BDEF, 0xFBF7E, 0xFFBFE, 0xFFFFE, 
                           0xFFFFF };

                           

const uint16_t SUB_BRIGHTNESS[] = {0x0000, 0x0008, 0x0208, 0x020A, 0x0A0A, 0x2A0A, 0x2A8A, 0x2AAA, 0xAAAA, 
    0xAABA, 0xABBA, 0xABBE, 0xEBBE, 0xEBBF, 0xFBBF, 0xFBFF, 0xFFFF };

void setup() {
  Serial.begin(115200);
  strip = Freenove_ESP32_WS2812(LED_COUNT, LED_PIN, CHANNEL, TYPE_GRB);
  strip.begin();
  strip.setBrightness(255);  

}

int linearize(float intensity) {
  /*
  a*exp(b*x)=y    
  a*exp(b*1)=1                            (1)
  a*exp(b*255)=16*128-1                   (2)
  
  (1)    a=1/exp(b)                       (3)
  (1+3)  1/exp(b)*exp(b*255)=16*128-1    
         exp(b*255)=(16*128-1)*exp(b)    
         ln(exp(b*255))=ln((16*128-1)*exp(b))    
         b*255=ln(16*128-1)+b    
         b*254=ln(16*128-1)    
         b=ln(16*128-1)/254    
  */
  const float b=log(16*128-1)/254;
  const float a=1/exp(b);
  
  return (int) (a*exp(b*intensity));
}

void setLEDs(int intensity) {
  uint8_t ledR, ledG, ledB;
  //Serial.printf("%d\n", intensity);
  int baselevel= intensity >> 4;
  int sublevel=intensity & 0xF;
  
  uint16_t controlBit=0x01;
  for(uint8_t led=0; led<16; led++) {
    ledR=(SUB_BRIGHTNESS[sublevel] & controlBit)? baselevel+1 : baselevel;
    ledG=ledR;
    ledB=ledR;
    strip.setLedColorData(led, ledR, ledG, ledB);
    controlBit=controlBit<<1;
  } // led
  strip.show();    
}

void loop() {
  for(float intensity=0; intensity<256; intensity++) {
    setLEDs(linearize(intensity));
    delay(5);
  }

  for(float intensity=255; intensity>0; intensity--) {
    setLEDs(linearize(intensity));
    delay(5);
  }

  setLEDs(linearize(0));
  delay(500);

  Serial.printf("%d => %d\n",    0, linearize(   0));
  Serial.printf("%d => %d\n",    1, linearize(   1));
  Serial.printf("%d => %d\n",    2, linearize(   2));
  Serial.printf("%d => %d\n",    3, linearize(   3));
  Serial.printf("%d => %d\n",  253, linearize( 253));
  Serial.printf("%d => %d\n",  254, linearize( 254));
  Serial.printf("%d => %d\n",  255, linearize( 255));
  Serial.printf("%d => %d\n",  256, linearize( 256));
}



volatile int interruptCounter;
int totalInterruptCounter;
 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
 
}
 
void setup() {
 
  Serial.begin(115200);
 
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
 
}
 
void loop() {
 
  if (interruptCounter > 0) {
 
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
 
    totalInterruptCounter++;
 
    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
 
  }
}
