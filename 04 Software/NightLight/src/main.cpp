/**************************************************************************
ToDo:
- re-implement clock
- import weather reports
- didcated screen handler
- smart refresh of screen
- debug why light can be flashing before switch off
- find limits on screen reset workaround
- display running light during startup

 **************************************************************************/
#include "Arduino.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "time.h"
#include "bitmaps.h"
#include "light.h"
#include "menu.h"
#include "key.h"
#include "clock.h"

/**************************************************************************
TYPES, CLASSES, ENUMS
**************************************************************************/


/**************************************************************************
 DEFINES AND CONSTANTS
**************************************************************************/
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define LED_COUNT 16
#define LED_PIN   23
#define CHANNEL    0

const char* ssid     = "Starbux";
const char* password = "ApplausjeVoorDeKok#";

/**************************************************************************
 GLOBAL VARIABLES
**************************************************************************/
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Light light(LED_COUNT, LED_PIN, CHANNEL, TYPE_GRB);
Clock myClock;

Tree menuTree;

Key leftButton ( 4, 50);
Key rightButton(12, 50);
Key topButton  (14, 50);

/**************************************************************************
FUNCTIONS
**************************************************************************/

void revitaliseDisplay(void) {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C, true);
  display.clearDisplay();
  display.setRotation(2);
}

void handlerFunction(actionID_t ID) {
  Serial.printf("Handler function called from action %d\n", ID);  
}

void colorHandler(actionID_t ID) {
  switch(ID) {
    case 10: light.setColor(lcWhite ); Serial.println("WHITE" );  break;
    case 11: light.setColor(lcYellow); Serial.println("YELLOW");  break;
    case 12: light.setColor(lcOrange); Serial.println("ORANGE");  break;
    case 13: light.setColor(lcRed   ); Serial.println("RED"   );  break;
  }
}

void intensityHandler(actionID_t ID) {
  switch(ID) {
    case 20: light.setIntensity(li25 ); break;
    case 21: light.setIntensity(li35 ); break;
    case 22: light.setIntensity(li50 ); break;
    case 23: light.setIntensity(li70 ); break;
    case 24: light.setIntensity(li100); break;
  }
  Serial.printf("Intensity %0f\n", light.getIntensity());  
}

void timerHandler(actionID_t ID) {
  switch(ID) {
    case 30: light.setTimer(td03);  break;
    case 31: light.setTimer(td05);  break;
    case 32: light.setTimer(td10);  break;
    case 33: light.setTimer(td20);  break;
    case 34: light.setTimer(tdOff); break;
  }
}

void menuHandler(actionID_t ID) {
  light.showClock();
}


/**************************************************************************
SETUP AND LOOP
**************************************************************************/

void setup() {
  Serial.begin(115200);

  // Get the time
  WiFi.begin(ssid, password);
  int attempts=0;
  while ((WiFi.status() != WL_CONNECTED) & (attempts<10)) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  Serial.println("\nWiFi connected.");

  //configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  //disconnect WiFi as it's no longer needed
  //WiFi.disconnect(true);
  //WiFi.mode(WIFI_OFF);


  Wire.begin(21, 22);

  // The display needs some time after a cold start
  delay(500);

  while(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, true)) { 
    delay(500);
    display.ssd1306_command(SSD1306_DISPLAYOFF);
    delay(500);
    display.ssd1306_command(SSD1306_DISPLAYON);
  }

  display.setRotation(2);

  // Build the menu structure
  menuTree.addScreen(&display);

  Tree* menuItem;

  menuTree.lowerLimit=20;

  menuItem=menuTree.addItem(1, "COLOR", &handlerFunction);
    menuItem->upperLimit=105;
    menuItem->addItem(10, "WHITE"     , &colorHandler);
    menuItem->addItem(11, "YELLOW"    , &colorHandler);
    menuItem->addItem(12, "ORANGE"    , &colorHandler);
    menuItem->addItem(13, "RED"       , &colorHandler);
    menuItem->select(0);
    colorHandler(10);

  menuItem=menuTree.addItem(2, "INTENSITY", &handlerFunction);
    menuItem->upperLimit=105;
    menuItem->addItem(20,  "25%" , &intensityHandler);
    menuItem->addItem(21,  "35%" , &intensityHandler);
    menuItem->addItem(22,  "50%" , &intensityHandler);
    menuItem->addItem(23,  "70%" , &intensityHandler);
    menuItem->addItem(24, "100%" , &intensityHandler);
    menuItem->select(2);
    intensityHandler(22);

  menuItem=menuTree.addItem(3, "TIMER" , &handlerFunction);
    menuItem->upperLimit=110;
    menuItem->addItem(30, "3 MIN"  , &timerHandler);
    menuItem->addItem(31, "5 MIN"  , &timerHandler);
    menuItem->addItem(32, "10 MIN" , &timerHandler);
    menuItem->addItem(33, "20 MIN" , &timerHandler);
    menuItem->addItem(34, "OFF"    , &timerHandler);
    menuItem->select(2);
    timerHandler(32);

  menuItem=menuTree.addItem(4, "BACK"  , &handlerFunction);
    menuItem->upperLimit=110;
    menuItem->addItem(40, "CLOSE MENU" , &menuHandler);

  menuTree.select(0);

  for(int i=0; i<3; i++) {
      revitaliseDisplay();
      delay(500);
  }

  light.off();
  light.on();
  light.showClock();
}

void loop() {

  Tree* topRow;
  Tree* btmRow;

  topRow=&menuTree;
  btmRow=topRow->selection();

  // Handle keys
  leftButton .scan();
  rightButton.scan();
  topButton  .scan();
  myClock    .scan();

  if (leftButton.pressed()) {

    if(light.menuSwitchedOn()) {
      // Only change rows if the screen was already switched on
      menuTree.next();
      btmRow=menuTree.selection();
    }
    light.showMenu(); // Switch on every time since it updates the timer
  }

  if (rightButton.pressed()) {

    if(light.menuSwitchedOn()) {
      // Only change rows if the screen was already switched on
      menuTree.selection()->next();
      menuTree.selection()->selection()->triggerCallback();
    }
    light.showMenu(); // Switch on every time since it updates the timer
  }

  if (topButton.pressed()) {
    light.toggle();
  }

  // Handle screen
  display.clearDisplay();

  if(light.menuSwitchedOn()) {
    topRow->draw(  1);
    btmRow->draw( 17);

    // Draw the arrow signs on bottom and top row
    display.fillRect(  0,  0, 16, 16, 0);
    display.fillRect(112, 16, 16, 16, 0);
    display.drawBitmap(  0,  0, right.data, right.width, right.height, 1);
    display.drawBitmap(114, 16, right.data, right.width, right.height, 1);
  }
  else {
    if (myClock.reliable()) {
      char text[25];
      struct tm timeinfo;
      if(getLocalTime(&timeinfo)) {
        sprintf(text, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
        display.setTextSize(2);
        display.setTextWrap(false);
        display.setTextColor(1); 
        display.setCursor(32,16);
        display.print(text);
        delay(100);
      }
    }
  }

  display.display();

  light.scan();
  //Serial.print(light.debug);
  
  static unsigned long elapsed=millis()+1000;
  if(millis()>elapsed) {
    //Serial.println(light.debug);
    elapsed=millis()+1000;
  }
}
