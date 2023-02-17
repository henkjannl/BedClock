#include <AsyncTelegram2.h>

// Timezone definition
#include <time.h>
#define MYTZ "CET-1CEST,M3.5.0,M10.5.0/3"

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <Wire.h>

#include "FS.h"
#include "SPIFFS.h"
#include <map>

#define VERSION "1.0.0"
/* 
1.0.0 Initial release

To do:
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

#define LED_COUNT   16
#define LED_PIN     23
#define LED_CHANNEL  0

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
  { lb15, 0.15 };
  { lb30, 0.30 },
  { lb50, 0.50 },
  { lb100, 1.00 } }

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
  bool lightOn = false;
  floatVariable_t R, G, B;

  void UpdateStatus() {
    if( lightOn ) {
      // If the user has changed bridghtess or color sett
      R.setTarget(MAX_INTENSITY*COLORS[color].R*BRIGHTNESSES[brightness]);
      G.setTarget(MAX_INTENSITY*COLORS[color].G*BRIGHTNESSES[brightness]);
      B.setTarget(MAX_INTENSITY*COLORS[color].B*BRIGHTNESSES[brightness]);
    }
    else {
      R.setTarget(0);
      G.setTarget(0);
      B.setTarget(0);
    } // if lightOn
  }
}

// ======== GLOBAL VARIABLES ============= 
WiFiMulti wifiMulti;
WiFiClientSecure client;  
AsyncTelegram2 myBot(client);
InlineKeyboard inlineKeyboard;
lightStatus_t lightStatus;
Freenove_ESP32_WS2812 strip;

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

// Callback functions definition for inline keyboard buttons
void onQueryReply(const TBMessage &queryMsg){
  switch( queryMsg.callbackQueryID ) {
    case CB_COLOR_WHITE:
      lightStatus.color = lcWhite;
    break;

    case CB_COLOR_YELLOW:
      lightStatus.color = lcYellow;
    break;

    case CB_COLOR_ORANGE:
      lightStatus.color = lcOrange;
    break;

    case CB_COLOR_RED:
      lightStatus.color = lcRed;
    break;

    case CB_TIME_3MIN:
      lightStatus.timer = lt03;
    break;

    case CB_TIME_5MIN:
      lightStatus.timer = lt05;
    break;

    case CB_TIME_10MIN:
      lightStatus.timer = lt10;
    break;

    case CB_TIME_20MIN:
      lightStatus.timer = lt20;
    break;

    case CB_BRIGHTNESS_15:
      lightStatus.brightness = lb15;
    break;

    case CB_BRIGHTNESS_30:
      lightStatus.brightness = lb30;
    break;

    case CB_BRIGHTNESS_50:
      lightStatus.brightness = lb50;
    break;

    case CB_BRIGHTNESS_100:
      lightStatus.brightness = lb100;
    break;

    case CB_LIGHT_ON:
      lightStatus.lightOn = true;
    break;

    case CB_LIGHT_OFF:
      lightStatus.lightOn = false;
    break;
  }

  // Set new target values for the light
  lightStatus.UpdateStatus();
}

void setup() {
  // initialize the Serial
  Serial.begin(115200);

  // Start the filesystem 
  SPIFFS.begin();
  
  WiFi.mode(WIFI_STA);

  // Add wifi access points 
  for (const auto &ap : ACCESS_POINTS ) {
    Serial.printf( "%s %s\n", ap.first.c_str(), ap.second.c_str() );
    wifiMulti.addAP( ap.first.c_str(), ap.second.c_str() );
  }

  Serial.println("Connecting Wifi...");
  if(wifiMulti.run() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("WiFi not connected yet");
  }

  WiFi.setAutoReconnect(true);

  // Sync time with NTP
  configTzTime(MYTZ, "time.google.com", "time.windows.com", "pool.ntp.org");
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
}


void loop() {
  time_t rawtime;
  struct tm * timeinfo;
  static int prev_min = -1;
  static bool wificonnectReported = false;

  // Reconnect wifi if required
  if(wifiMulti.run() != WL_CONNECTED) {
    if (!wificonnectReported) {
      Serial.println("WiFi not connected!");
      wificonnectReported=true;
    } 
  } 
  else {
    wificonnectReported=false;
  }

  const unsigned long eventInterval = 100;
  static unsigned long previousTime = millis();
  unsigned long currentTime = millis();

  if (currentTime - previousTime >= eventInterval) {
    previousTime = currentTime;
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
