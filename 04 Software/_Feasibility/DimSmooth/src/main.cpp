#include <Arduino.h>
#include <FastLED.h>

/* ---------------------
// DEFINES
// --------------------- */
#define LED_PIN     23
#define COLOR_ORDER RGB
#define CHIPSET     WS2812B
#define NUM_LEDS    16

#define BRIGHTNESS  255
#define FRAMES_PER_SECOND 120

#define DEBUG_MY_LEDS true

// ---------------------
// CONSTANTS
// --------------------- 

// ---------------------
// GLOBAL VARIABLES
// --------------------- 
CRGB leds[NUM_LEDS];

// ---------------------
// FUNCTIONS
// --------------------- 
void setColor(uint16_t brightness) {

  // Determine which leds to give an additional brightness increment
  const uint16_t BRIGHTNESS_LEVELS[] = {
    0x0000, 0x0002, 0x0082, 0x4082, 0x4182, 0x4186, 0x4986, 0x6986, 
    0x6996, 0x699E, 0x6B9E, 0xEB9E, 0xEBBE, 0xFBBE, 0xFBFE, 0xFBFF
  };

  uint16_t mask = BRIGHTNESS_LEVELS[brightness & 0xF];
  uint8_t brght = brightness>>4;

  CRGB brighter = CRGB(brght+1, brght+1, brght+1);
  CRGB duller   = CRGB(brght,   brght,   brght  );

  if(DEBUG_MY_LEDS) Serial.printf("%04d ", brightness );

  // Switch individual LEDs to the higher or lower intensity, based on the sub-brightness mask
  for(int i=0; i<NUM_LEDS; i++) {
    leds[i]=((mask & 0b1) == 0) ?  duller : brighter;
    if(DEBUG_MY_LEDS) Serial.printf("%03d ", leds[i].r );
    mask=mask >> 1;
  }
  if(DEBUG_MY_LEDS) Serial.println();
}



void setup() {
  Serial.begin(115200);
  delay(250); // sanity delay
  
}

void loop()
{
  static uint16_t brightness=0;
  static bool goingUp = true;

  if(goingUp) {
    brightness++;
    if(brightness==0x200) goingUp=false;
  }
  else {
    brightness--;
    if(brightness==0x000) goingUp=true;
  }  

  setColor(brightness);

  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}
