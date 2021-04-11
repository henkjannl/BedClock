#include "Freenove_WS2812_Lib_for_ESP32.h"
#include <list>
#include <vector>
#include <iterator>

#define LEDS_COUNT  16
#define LEDS_PIN	23
#define CHANNEL		0

Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

volatile bool interruptEvent;
float target=0.6;
std::list<float> targets{ 0.3, 0.9, 0.5, 0.05, 0.01 };


hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
 
void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptEvent=true;
  portEXIT_CRITICAL_ISR(&timerMux); 
}
 
void setup() {
  // Start serial 
  Serial.begin(115200);

  // Start LED grid
  strip.begin();

  // Start fast timer
  timer = timerBegin(0, 80, true); // prescaler of 80 makes the timer tick at 1 µs
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 100, true); // 10000 corresponds to 10 ms or 100 Hz
  //timerAlarmWrite(timer, 1000000, true); // 1000000 corresponds to 1000 ms or 1 Hz
  timerAlarmEnable(timer);
}
 
void loop() {
  static unsigned long millisElapsed=0;
  static unsigned long events=0;
  bool higher;
  static float intensity=0;
  
  if (interruptEvent) {
 
    portENTER_CRITICAL(&timerMux);
    interruptEvent=false;
    portEXIT_CRITICAL(&timerMux);

    events++;

    higher=(intensity<target);
    intensity=0.99*intensity+0.01*(higher?1:0);

    int j=higher?20:0;
    for (int i = 0; i < LEDS_COUNT; i++) {
      strip.setLedColorData(i, j, j, j);
    }
    strip.show();
  }

  if(millis()>millisElapsed) {
    millisElapsed=millis()+5000;
    Serial.printf("Events: %d Intensity: %.3f Target: %.3f\n", events, intensity, target);
    target=targets.front();
    targets.pop_front();
    targets.push_back(target);
    Serial.printf("New target:%.3f\n", target);
  }
}
