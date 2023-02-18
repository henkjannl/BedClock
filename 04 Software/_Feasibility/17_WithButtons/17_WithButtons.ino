#include <Freenove_WS2812_Lib_for_ESP32.h>

#include <AsyncTelegram2.h>

// Timezone definition
#include <time.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <Wire.h>

#include "FS.h"
#include "SPIFFS.h"
#include <map>

#include <U8g2lib.h>

#define VERSION "1.0.0"
/* 
1.0.0 Initial release

To do:
Center time on screen
Add screen brightness
Add screen brightness to Telegram settings
Make settings persistent in SPIFFS
Add weather
Switch back to FreeRTOS
*/
 
/*
MyCredentials.h is in gitignore and should have the following content:

const char* token =  "aaaaaaaaaa:bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  // Telegram token
int64_t userid = 12345678; // User who receives a message after startup

std::map<String, String > ACCESS_POINTS { 
  {"SSID-1", "Password-1" }, 
  {"SSID-2", "Password-2" }, 
  {"SSID-3", "Password-3" } };
*/
#include "MyCredentials.h"

#define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3"

#define LED_COUNT   16
#define LED_PIN     23
#define LED_CHANNEL  0

#define PIN_KEY_LEFT   4
#define PIN_KEY_RIGHT 12
#define PIN_KEY_TOP   14
#define KEY_TRESHOLD  50

// Messages for he callback functions
#define CB_COLOR_WHITE     "clrWhite"
#define CB_COLOR_YELLOW    "clrYellow"
#define CB_COLOR_ORANGE    "clrOrange" 
#define CB_COLOR_RED       "clrRed"
#define CB_TIME_3MIN       "tim3min" 
#define CB_TIME_5MIN       "tim5min" 
#define CB_TIME_10MIN      "tim10min" 
#define CB_TIME_20MIN      "tim20min" 
#define CB_BRIGHTNESS_15   "brt15" 
#define CB_BRIGHTNESS_30   "brt30" 
#define CB_BRIGHTNESS_50   "brt50" 
#define CB_BRIGHTNESS_100  "brt100" 
#define CB_LIGHT_ON        "lgtOn" 
#define CB_LIGHT_OFF       "lgtOff" 

// Emoticons to spice up the Telegram messages
const char EMOTICON_DROPLETS[]    = { 0xf0, 0x9f, 0x92, 0xa6, 0x00 };
const char EMOTICON_TEMPERATURE[] = { 0xf0, 0x9f, 0x8c, 0xa1, 0x00 };
const char EMOTICON_PRESSURE[]    = { 0xf0, 0x9f, 0x8c, 0xaa, 0x00 };
const char EMOTICON_WELCOME[]     = { 0xf0, 0x9f, 0x99, 0x8b, 0xe2, 0x80, 0x8d, 0xe2, 0x99, 0x80, 0xef, 0xb8, 0x8f, 0x00 };

const float LIGHT_INC[] = { 0.0008, 0.0056, 0.0148, 0.0274, 0.0422, 0.0578, 0.0726, 0.0852, 0.0944, 0.0992, 0.0992, 
  0.0944, 0.0852, 0.0726, 0.0578, 0.0422, 0.0274, 0.0148, 0.0056, 0.0008};

const int MAX_INC = 19;

const float MAX_INTENSITY = 255.0;

const uint32_t connectTimeoutMs = 10000;

// The order in which the LEDs in the 4x4 grid are switched on
const uint16_t SUB_BRIGHTNESS[] = {0x0000, 0x0008, 0x0208, 0x020A, 0x0A0A, 0x2A0A, 0x2A8A, 0x2AAA, 0xAAAA, 
    0xAABA, 0xABBA, 0xABBE, 0xEBBE, 0xEBBF, 0xFBBF, 0xFBFF };

enum lightColor_t      { lcWhite, lcYellow, lcOrange, lcRed };
enum lightBrightness_t { lb15, lb30, lb50, lb100 };
enum lightTimer_t      { lt03, lt05, lt10, lt20 };

struct rgb_t { 
  float R;
  float G;
  float B; 
};

std::map<lightColor_t, rgb_t> COLORS = { 
  { lcWhite,   { 1.000, 1.000, 1.000 } },   
  { lcYellow,  { 1.000, 1.000, 0.500 } },   
  { lcOrange,  { 1.000, 0.750, 0.500 } },   
  { lcRed,     { 1.000, 0.500, 0.500 } }
};

std::map<lightBrightness_t, float> BRIGHTNESSES = { 
  { lb15,  0.15 },
  { lb30,  0.30 },
  { lb50,  0.50 },
  { lb100, 1.00 } };

std::map<lightTimer_t, unsigned int> TIMES = { 
  { lt03,  3*60*1000 }, 
  { lt05,  5*60*1000 },
  { lt10, 10*60*1000 },
  { lt20, 20*60*1000 }
};

// This class is used to smoothly control the values for the RGB channels of the light
class floatVariable_t {

  public:
    floatVariable_t (){ 
      _value=0;
      _amplitude=0;
      _index=MAX_INC+1;
      _ready=true;
    };    
    
    float getValue()  { return _value; };
    float getTarget() { return _target; };

    // Directly set value without steps
    void setValue(float value){ 
      _value=value; 
      _target=value;
      _ready=true;
    };

    // Go to target with motion steps
    void setTarget(float target) { 
      if( abs(target-_target)>0.001) { // New target is significantly different from previous target
        _amplitude=target-_value;
        _target=target;
        _index=0;
        _ready=false;
      };
    };

    // Do next step
    void step() { 
      if(!_ready) { 
        _value+=_amplitude*LIGHT_INC[_index]; 
        _index++; 
        if(_index>MAX_INC) _ready=true;
      }; // !_ready
    };

    bool ready() { return _ready; };

  private:
    float _value;
    float _target;
    float _amplitude;
    byte _index;
    bool _ready; 
}; // class floatVariable_t

struct lightStatus_t {
  lightColor_t color = lcWhite;
  lightBrightness_t brightness = lb50;
  lightTimer_t timer = lt03;
  unsigned long previousTimer;
  bool lightOn = false;
  floatVariable_t R, G, B;

  void UpdateStatus() {
    if( lightOn ) {
      // If the user has changed bridghtess or color sett
      R.setTarget(MAX_INTENSITY*COLORS[color].R*BRIGHTNESSES[brightness]);
      G.setTarget(MAX_INTENSITY*COLORS[color].G*BRIGHTNESSES[brightness]);
      B.setTarget(MAX_INTENSITY*COLORS[color].B*BRIGHTNESSES[brightness]);
      Serial.printf( "R: %.3f G: %.3f B:%.3f\n", R.getTarget(), G.getTarget(), B.getTarget() );
    }
    else {
      R.setTarget(0);
      G.setTarget(0);
      B.setTarget(0);
    } // if lightOn
  }; // UpdateStatus()

  void LightOn() {
    lightOn = true;
    previousTimer = millis();
    UpdateStatus();
  }; // LightOn()
  
  void SwitchLight() {
    lightOn = !lightOn;
    previousTimer = millis();
    UpdateStatus();
  }
};

// ======== GLOBAL VARIABLES ============= 
WiFiMulti wifiMulti;
WiFiClientSecure client;  
AsyncTelegram2 myBot(client);
InlineKeyboard inlineKeyboard;
lightStatus_t lightStatus;
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LED_COUNT, LED_PIN, LED_CHANNEL, TYPE_GRB);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R2, 22, 21, U8X8_PIN_NONE);  

String convertToHexString( String input ) {
  String result = "{ ";
  for( int i=0; i<input.length(); i++ ) {
    result += String("0x") + String( (int) input[i], HEX ) + ", ";
  }
  result+="0x00 };";
  result.toLowerCase();
  Serial.println(input + ": " + result);
  return result;
}

void setScreenBrightness(uint8_t brightness) {
  /* Typical values for the parameters:
   *    setScreenBrightness(255, 0,  1, 1); // most bright
   *    setScreenBrightness( 16, 0,  1, 1);
   *    setScreenBrightness(  5, 0,  0, 0);
   *    setScreenBrightness(  3, 0,  0, 0);
   *    setScreenBrightness(  1, 0,  0, 0); // least bright   */
  uint8_t contrast, vcom, p1, p2;

  switch( brightness ) {
    case 0: contrast = 255; vcom=0; p1=1; p2=1; break;
    case 1: contrast =  16; vcom=0; p1=1; p2=1; break;
    case 2: contrast =   5; vcom=0; p1=0; p2=0; break;
    case 3: contrast =   3; vcom=0; p1=0; p2=0; break;
    case 5: contrast =   1; vcom=0; p1=0; p2=0; break;
  }
   
  u8g2.setContrast(contrast);
  u8x8_cad_StartTransfer(u8g2.getU8x8());
  u8x8_cad_SendCmd(u8g2.getU8x8(), 0x0db);
  u8x8_cad_SendArg(u8g2.getU8x8(), vcom << 4);
  u8x8_cad_SendCmd(u8g2.getU8x8(), 0x0d9);
  u8x8_cad_SendArg(u8g2.getU8x8(), (p2 << 4) | p1 );
  u8x8_cad_EndTransfer(u8g2.getU8x8());
}

// Callback functions definition for inline keyboard buttons
void onQueryReply(const TBMessage &queryMsg){
  if( queryMsg.callbackQueryData == CB_COLOR_WHITE ) {
    Serial.println("Color white");
    lightStatus.color = lcWhite;
  }
  else if( queryMsg.callbackQueryData == CB_COLOR_YELLOW ) {
    Serial.println("Color yellow");
    lightStatus.color = lcYellow;
  }
  else if( queryMsg.callbackQueryData == CB_COLOR_ORANGE ) {
    Serial.println("Color orange");
    lightStatus.color = lcOrange;
  }
  else if( queryMsg.callbackQueryData == CB_COLOR_RED ) {
    Serial.println("Color red");
    lightStatus.color = lcRed;
  }
  else if( queryMsg.callbackQueryData == CB_TIME_3MIN ) {
    Serial.println("Timer 3 minutes");
    lightStatus.timer = lt03;
  }
  else if( queryMsg.callbackQueryData == CB_TIME_5MIN ) {
    Serial.println("Timer 5 minutes");
    lightStatus.timer = lt05;
  }
  else if( queryMsg.callbackQueryData == CB_TIME_10MIN ) {
    Serial.println("Timer 10 minutes");
    lightStatus.timer = lt10;
  }
  else if( queryMsg.callbackQueryData == CB_TIME_20MIN ) {
    Serial.println("Timer 20 minutes");
    lightStatus.timer = lt20;
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_15 ) {
    Serial.println("Brightness 15%");
    lightStatus.brightness = lb15;
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_30 ) {
    Serial.println("Brightness 30%");
    lightStatus.brightness = lb30;
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_50 ) {
    Serial.println("Brightness 50%");
    lightStatus.brightness = lb50;
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_100 ) {
    Serial.println("Brightness 100%");
    lightStatus.brightness = lb100;
  }
  else if( queryMsg.callbackQueryData == CB_LIGHT_ON ) {
    Serial.println("Bedlight on");
    lightStatus.LightOn();
  }
  else if( queryMsg.callbackQueryData == CB_LIGHT_OFF ) {
    Serial.println("Bedlight off");
    lightStatus.lightOn = false;
  }

  // Set new target values for the light
  lightStatus.UpdateStatus();
}

void setup() {
  // initialize the Serial
  Serial.begin(115200);

  // Start the filesystem 
  SPIFFS.begin();

  // Initialize light
  Serial.println( "Initialize light" );
  strip.begin();
  strip.setBrightness(255);    

  // Initialize display
  Serial.println( "Initialize display" );
  u8g2.initDisplay();
  delay(500);
  u8g2.begin();

  // Initialize Wifi
  Serial.println( "Initialize WiFi" );
  WiFi.mode(WIFI_STA);

  // Add wifi access points 
  for (const auto &ap : ACCESS_POINTS ) {
    Serial.printf( "%s %s\n", ap.first.c_str(), ap.second.c_str() );
    wifiMulti.addAP( ap.first.c_str(), ap.second.c_str() );
  }

  Serial.println("Connecting Wifi...");
  if(wifiMulti.run( connectTimeoutMs ) == WL_CONNECTED) {
    Serial.printf( "WiFi connected to SSID %s signl strength %ddB\n", WiFi.SSID().c_str(), WiFi.RSSI() );
  }
  else {
    Serial.println("WiFi not connected yet");
  }

  WiFi.setAutoReconnect(true);

  // Sync time with NTP
  Serial.println("Sync clock with timeserver");
  configTzTime(MYTZ, "time.google.com", "time.windows.com", "pool.ntp.org");
  
  // Initialize Telegram
  Serial.println("Setup Telegram");
  client.setCACert(telegram_cert);

  // Set the Telegram bot properties
  myBot.setUpdateTime(1000);
  myBot.setTelegramToken(token);
  myBot.setMyCommands("/start", "start conversation");

  // Check if all things are ok
  Serial.print("Test Telegram connection... ");
  myBot.begin() ? Serial.println("OK") : Serial.println("NOK");
  Serial.print("Bot name: @");
  Serial.println(myBot.getBotName());

  // Add sample inline keyboard
  inlineKeyboard.addButton("On",     CB_LIGHT_ON,       KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addButton("Off",    CB_LIGHT_OFF,      KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addRow();
  inlineKeyboard.addButton("White",  CB_COLOR_WHITE,    KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addButton("3 min",  CB_TIME_3MIN,      KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addButton("15%",    CB_BRIGHTNESS_15,  KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addRow();
  inlineKeyboard.addButton("Yellow", CB_COLOR_YELLOW,   KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addButton("5 min",  CB_TIME_5MIN,      KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addButton("30%",    CB_BRIGHTNESS_30,  KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addRow();
  inlineKeyboard.addButton("Orange", CB_COLOR_ORANGE,   KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addButton("10 min", CB_TIME_10MIN,     KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addButton("50%",    CB_BRIGHTNESS_50,  KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addRow();
  inlineKeyboard.addButton("Red",    CB_COLOR_RED,      KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addButton("20 min", CB_TIME_20MIN,     KeyboardButtonQuery, onQueryReply);
  inlineKeyboard.addButton("100%",   CB_BRIGHTNESS_100, KeyboardButtonQuery, onQueryReply);

  // Add pointer to this keyboard to bot (in order to run callback function)
  myBot.addInlineKeyboard(&inlineKeyboard);

  String text = String(EMOTICON_WELCOME) + " Welcome!";
  myBot.sendTo(userid, text.c_str(), inlineKeyboard.getJSON() );

  Serial.println("End of setup loop");
}


void loop() {
  static bool wifiConnectReported = false;
  static bool wifiNotConnectReported = false;

  // Check if WiFi is still alive
  if (wifiMulti.run( connectTimeoutMs ) == WL_CONNECTED) {  /*if the connection lost it will connect to next network*/
    if( !wifiConnectReported ) {
      Serial.printf( "WiFi connected to SSID %s signl strength %ddB\n", WiFi.SSID().c_str(), WiFi.RSSI() );
      wifiConnectReported=true;
      wifiNotConnectReported=false;
    };
  }
  else {
    if( !wifiNotConnectReported ) {
      Serial.println("WiFi not connected!");  /*if all conditions fail print this*/
      wifiNotConnectReported=true;
      wifiConnectReported=false;
    }
  }

  static volatile uint32_t keyLeftCounter  = 0;
  static volatile uint32_t keyRightCounter = 0;
  static volatile uint32_t keyTopCounter   = 0;

  // Check if button is pressed
  if (touchRead(PIN_KEY_LEFT )<KEY_TRESHOLD) keyLeftCounter++;  else keyLeftCounter =0; 
  if (touchRead(PIN_KEY_TOP  )<KEY_TRESHOLD) keyTopCounter++;   else keyTopCounter  =0; 
  if (touchRead(PIN_KEY_RIGHT)<KEY_TRESHOLD) keyRightCounter++; else keyRightCounter=0; 

  if(keyLeftCounter ==3) { /* Nothing to do yet */ };
  if(keyTopCounter  ==3) { lightStatus.SwitchLight();  };
  if(keyRightCounter==3) { /* Nothing to do yet */ };

  // Update clock
  time_t rawtime;
  struct tm * timeinfo;
  static int prev_min = -1;
  static bool firstClockUpdate = true;
  
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  if( firstClockUpdate or ( ( timeinfo->tm_year > 80 ) and ( timeinfo->tm_min != prev_min ) ) ) {
    prev_min = timeinfo->tm_min;
    firstClockUpdate = false;
    char timestr[10];
    snprintf(timestr, 10, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min );
    Serial.printf("Update time %s\n", timestr);
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_cu12_tr);    
    u8g2.setFontPosCenter();
    u8g2.setCursor( ( (u8g2.getDisplayWidth() - (u8g2.getUTF8Width(timestr))) / 2) , 16);
    u8g2.print( timestr );    
    u8g2.sendBuffer();
  }
  
  // Increase or decrease light intensity in small timesteps
  unsigned long currentTime = millis();
  const unsigned long lightDimInterval = 100;
  static unsigned long previousLightDim = millis();

  if (currentTime - previousLightDim >= lightDimInterval ) {
    previousLightDim = currentTime;
    // Get the integer component
    uint16_t bR=(uint16_t) lightStatus.R.getValue();
    uint16_t bG=(uint16_t) lightStatus.G.getValue();
    uint16_t bB=(uint16_t) lightStatus.B.getValue();

    // Translate into sublevels for the individual levels
    uint8_t  baseR = bR>>4;  uint16_t subR = SUB_BRIGHTNESS[bR & 0xF]; 
    uint8_t  baseG = bG>>4;  uint16_t subG = SUB_BRIGHTNESS[bG & 0xF]; 
    uint8_t  baseB = bB>>4;  uint16_t subB = SUB_BRIGHTNESS[bB & 0xF]; 

    uint8_t ledR, ledG, ledB;
    uint16_t controlBit=0x01;
   
    for(uint8_t i=0; i<16; i++) {
        ledR=baseR;
        ledG=baseG; 
        ledB=baseB;

        if( (subR & controlBit) !=0 ) ledR++;
        if( (subG & controlBit) !=0 ) ledG++;
        if( (subB & controlBit) !=0 ) ledB++;

        controlBit=controlBit<<1;

        strip.setLedColorData(i, ledR, ledG, ledB);        
    } // for i

    strip.show();    

    lightStatus.R.step();
    lightStatus.G.step();
    lightStatus.B.step();
  }  // millis()

  // Switch off the light if the timer elapses
  static unsigned long previousTimer = millis();
  if ( lightStatus.lightOn and (currentTime - lightStatus.previousTimer >= TIMES[lightStatus.timer] ) ) {
    lightStatus.lightOn = false;
    lightStatus.UpdateStatus();
  };

  // a variable to store telegram message data
  TBMessage msg;

  // if there is an incoming message...
  if (myBot.getNewMessage(msg)) {
    // check what kind of message I received
    String tgReply;
    MessageType msgType = msg.messageType;
    
    switch (msgType) {
      case MessageText :
        // received a text message
        tgReply = msg.text;
        Serial.print("Text message received: ");
        Serial.println(tgReply);

        if (tgReply.equalsIgnoreCase("/start")) {          
          myBot.sendMessage(msg, "Welcome!", inlineKeyboard);      
          Serial.printf("Start command received from %d", msg.chatId);    
        }        
        else {
          // write back feedback message and show a hint
          String text = String("const char EMOTICON[] = ") + convertToHexString( msg.text );
          myBot.sendMessage(msg, text.c_str(), inlineKeyboard);


          
        }
        break;
        
        /* 
        * Telegram "inline keyboard" provide a callback_data field that can be used to fire a callback fucntion
        * associated at every inline keyboard buttons press event and everything can be handled in it's own callback function. 
        * Anyway, is still possible poll the messagetype in the same way as "reply keyboard" or both.              
        */
        case MessageQuery:
          break;
        
        default:
          break;
    }
  }
}
