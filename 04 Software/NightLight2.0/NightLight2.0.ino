#include <Arduino.h>
#include "esp_system.h"

#include <stdlib.h>     

#if CONFIG_FREERTOS_UNICORE
  #define ARDUINO_RUNNING_CORE 0
#else
  #define ARDUINO_RUNNING_CORE 1
#endif

#include "data.h"
#include "keyboard.h"
#include "screen.h"
#include "light.h"

using namespace std;

void setup(void) {
  Serial.begin(115200);
  setupKeyboard();
  setupScreen();
  setupLight();
  
}

void loop(void) {

}
