#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <AsyncTelegram2.h>

#include <map>
#include "MyCredentials.h"
#include "weather.h"

#include <Wire.h>
#include <Freenove_WS2812_Lib_for_ESP32.h>
#include <U8g2lib.h>

#include "FS.h"
#include "SPIFFS.h"
#include <time.h>
#include <vector>

#define VERSION "1.0.0"
/* 
1.0.0 Initial release

To do:
  Add precipitation
  Vastly reorganize code
  Switch back to FreeRTOS
  Allow over the air updates
  Change LIGHT_INC to function
  
*/
 
/*
MyCredentials.h is in gitignore and should have the following content:

// Telegram token for the Bedlight bot
const char* token =  "aaaaaaaaaa:bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";  

// Telegram user ID for the user to be notified on startup
int64_t userid = 12345678; 

// Timezone where the bedlight is located
// https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
#define localTimezone "CET-1CEST,M3.5.0,M10.5.0/3"

// List of SSID, password combinations for WiFi access points to connect to
std::map<String, String > ACCESS_POINTS { 
  {"SSID-1", "Password-1" }, 
  {"SSID-2", "Password-2" }, 
  {"SSID-3", "Password-3" } };
*/

#define LED_COUNT     16
#define LED_PIN       23
#define LED_CHANNEL    0

#define PIN_KEY_LEFT   4
#define PIN_KEY_RIGHT 12
#define PIN_KEY_TOP   14
#define KEY_TRESHOLD  50

// For storing settings in SPIFFS
#define SETTINGS_FILE "/settings.jsn"
#define SETTINGS_TEMP "/settings.tmp"

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
#define CB_SETTINGS        "settings" 
#define CB_SCREEN_BR_1     "scnbrht1" 
#define CB_SCREEN_BR_2     "scnbrht2" 
#define CB_SCREEN_BR_3     "scnbrht3" 
#define CB_SCREEN_BR_4     "scnbrht4" 
#define CB_SCREEN_BR_5     "scnbrht5" 
#define CB_MAIN_MENU       "mainmenu" 

// Emoticons to spice up the Telegram messages
const char EMOTICON_WELCOME[]     = { 0xf0, 0x9f, 0x99, 0x8b, 0xe2, 0x80, 0x8d, 0xe2, 0x99, 0x80, 0xef, 0xb8, 0x8f, 0x00 };
const char EMOTICON_WHITE[]       = { 0xe2, 0x9a, 0xaa, 0xef, 0xb8, 0x8f, 0x00 };
const char EMOTICON_YELLOW[]      = { 0xf0, 0x9f, 0x9f, 0xa1, 0x00 };
const char EMOTICON_ORANGE[]      = { 0xf0, 0x9f, 0x9f, 0xa0, 0x00 };
const char EMOTICON_RED[]         = { 0xf0, 0x9f, 0x94, 0xb4, 0x00 };
const char EMOTICON_TIMER[]       = { 0xe2, 0x8f, 0xb2, 0x00 };
const char EMOTICON_BRIGHTNESS[]  = { 0xf0, 0x9f, 0x94, 0x86, 0x00 };
const char EMOTICON_SETTINGS[]    = { 0xe2, 0x9a, 0x99, 0xef, 0xb8, 0x8f, 0x00 };
const char EMOTICON_LIGHT_ON[]    = { 0xf0, 0x9f, 0x92, 0xa1, 0x00 };
const char EMOTICON_LIGHT_OFF[]   = { 0xf0, 0x9f, 0x98, 0xb4, 0x00 };
const char EMOTICON_MAIN_MENU[]   = { 0xf0, 0x9f, 0x8f, 0xa0, 0x00 };

const char DEGREE_SYMBOL[]        = { 0xB0, '\0' };

// Smooth light increments. It is the incremental derivative of x-1/(2*pi)*sin(2*pi*x) in the domain x=0..1. The sum is 1. 
std::vector<float> LIGHT_INC = { 0.0008, 0.0056, 0.0148, 0.0274, 0.0422, 0.0578, 0.0726, 0.0852, 0.0944, 0.0992, 0.0992, 
  0.0944, 0.0852, 0.0726, 0.0578, 0.0422, 0.0274, 0.0148, 0.0056, 0.0008};

const float    MAX_INTENSITY = 255.0;
const uint32_t CONNECT_TIMEOUT_MS = 10000;

// The order in which the LEDs in the 4x4 grid are switched on
const uint16_t SUB_BRIGHTNESS[] = {0x0000, 0x0008, 0x0208, 0x020A, 0x0A0A, 0x2A0A, 0x2A8A, 0x2AAA, 0xAAAA, 
    0xAABA, 0xABBA, 0xABBE, 0xEBBE, 0xEBBF, 0xFBBF, 0xFBFF };

enum lightColor_t       { lcWhite, lcYellow, lcOrange, lcRed };
enum lightBrightness_t  { lb15, lb30, lb50, lb100 };
enum lightTimer_t       { lt03, lt05, lt10, lt20 };
enum keyboard_t         { kbMain, kbSetings };
enum screenBrightness_t { sb1, sb2, sb3, sb4, sb5 };
enum screen_t           { scnMain, scnWeather };

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

std::map<lightTimer_t, String > TIMES_STR = { 
  { lt03, "3 minutes"  }, 
  { lt05, "5 minutes"  },
  { lt10, "10 minutes" },
  { lt20, "20 minutes" }
};

// This class is used to smoothly control the values for the RGB channels of the light
class floatVariable_t {

  public:
    floatVariable_t (){ 
      _value=0;
      _amplitude=0;
      _index=LIGHT_INC.size();
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
        if(_index>LIGHT_INC.size()-1) _ready=true;
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

class lightStatus_t {
  public:
    // Status of the light
    bool              lightOn    = false;
    lightColor_t      color      = lcWhite;
    lightBrightness_t brightness = lb50;
    lightTimer_t      timer      = lt03;
  
    // Screen brightness
    screenBrightness_t screenBrightness = sb4;

    // Which screen to display
    screen_t screen = scnMain;

    // Need to save settings 
    bool settingsChanged = false;
    
    // Current menu
    keyboard_t menu = kbMain;

    floatVariable_t R, G, B;
  
    unsigned long previousTimer;
  
    void UpdateStatus() {
      if( lightOn ) {
        // If the user has changed bridghtess or color sett
        R.setTarget(MAX_INTENSITY*COLORS[color].R*BRIGHTNESSES[brightness]);
        G.setTarget(MAX_INTENSITY*COLORS[color].G*BRIGHTNESSES[brightness]);
        B.setTarget(MAX_INTENSITY*COLORS[color].B*BRIGHTNESSES[brightness]);
        Serial.printf( "New light target: R: %.3f G: %.3f B:%.3f\n", R.getTarget(), G.getTarget(), B.getTarget() );
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
    
    void LightOff() {
      lightOn = false;
      UpdateStatus();
    }; // LightOff()
    
    void SwitchLight() {
      lightOn = !lightOn;
      previousTimer = millis();
      UpdateStatus();
    }

    void saveSettings(fs::FS &fs, const char * tempFile, const char * settingsFile) {    
    
      // Saves the settings to a file
      StaticJsonDocument<1024> doc;
    
      File output = fs.open(tempFile, FILE_WRITE);

      doc[ "lightOn"            ] = lightOn;
      doc[ "lightColor"         ] = (int) color;    
      doc[ "lightBrightness"    ] = (int) brightness;    
      doc[ "timer"              ] = (int) timer;    
      doc[ "screenBrightness"   ] = (int) screenBrightness;
        
      if( serializeJson(doc, output) > 0) {
        // Assume success if at least one byte was written
    
        // Remove settings file to temp file and replace settings file
        fs.remove(settingsFile);
        fs.rename(tempFile, settingsFile);
        Serial.printf("Saved settings to '%s'\n", settingsFile);
      }
    
      settingsChanged = false;
    } 

    void loadSettings(fs::FS &fs, const char * settingsFile) {    
      // Retrieve configuration data from the configuration file stored in SPIFFS 
      StaticJsonDocument<1024> doc;
    
      File input = fs.open(settingsFile);
    
      if(!input || input.isDirectory()){
          Serial.printf("- failed to open settings file %s for reading\n", settingsFile);
          return;
      }
      
      DeserializationError error = deserializeJson(doc, input);
      
      if (error) {
        Serial.print(F("deserializeJson() of settingsfile failed: "));
        Serial.println(error.f_str());
        return;
      }

      lightOn          = doc[ "lightOn"            ];
      color            = (lightColor_t)       doc[ "lightColor"         ].as<int>();
      brightness       = (lightBrightness_t)  doc[ "lightBrightness"    ].as<int>();
      timer            = (lightTimer_t)       doc[ "timer"              ].as<int>();
      screenBrightness = (screenBrightness_t) doc[ "screenBrightness"   ].as<int>();
    
      settingsChanged = false;

      Serial.println("Settings loaded");  
    };
    
};

// ======== GLOBAL VARIABLES ============= 
WiFiMulti wifiMulti;
WiFiClientSecure client;  
AsyncTelegram2 myBot(client);
InlineKeyboard mainKeyboard, settingsKeyboard;
lightStatus_t lightStatus;
weather_t weather;
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LED_COUNT, LED_PIN, LED_CHANNEL, TYPE_GRB);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R2, 22, 21, U8X8_PIN_NONE);  

std::map<keyboard_t, InlineKeyboard* > KEYBOARDS = { 
  { kbMain,    &mainKeyboard     },
  { kbSetings, &settingsKeyboard }
};

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

String StatusMessage() {
  String result;
  result = String(EMOTICON_LIGHT_ON) + lightStatus.lightOn ? " On " : " Off ";
  switch( lightStatus.color ) {
    case lcWhite:  result+= EMOTICON_WHITE;  break;
    case lcYellow: result+= EMOTICON_YELLOW; break;
    case lcOrange: result+= EMOTICON_ORANGE; break;
    case lcRed:    result+= EMOTICON_RED;    break;
  }

   
  result += String(" ") + EMOTICON_BRIGHTNESS + " " + String(100*BRIGHTNESSES[lightStatus.brightness],0)+"%";
  result += String(" ") + EMOTICON_TIMER + " " + TIMES_STR[lightStatus.timer];
  return result;
};  
  
void setScreenBrightness(screenBrightness_t brightness) {
  /* Typical values for the parameters:
   *    setScreenBrightness(255, 0,  1, 1); // most bright
   *    setScreenBrightness( 16, 0,  1, 1);
   *    setScreenBrightness(  5, 0,  0, 0);
   *    setScreenBrightness(  3, 0,  0, 0);
   *    setScreenBrightness(  1, 0,  0, 0); // least bright   */
  uint8_t contrast, vcom, p1, p2;

  switch( brightness ) {
    case sb1: contrast =   1; vcom=0; p1=0; p2=0; break;
    case sb2: contrast =   3; vcom=0; p1=0; p2=0; break;
    case sb3: contrast =   5; vcom=0; p1=0; p2=0; break;
    case sb4: contrast =  16; vcom=0; p1=1; p2=1; break;
    case sb5: contrast = 255; vcom=0; p1=1; p2=1; break;
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

  String newMessage = "Command not recognized";

  if( queryMsg.callbackQueryData == CB_COLOR_WHITE ) {
    newMessage = "The color of the light is set to white";
    lightStatus.color = lcWhite;
  }
  else if( queryMsg.callbackQueryData == CB_COLOR_YELLOW ) {
    newMessage = "The color of the light is set to yellow";
    lightStatus.color = lcYellow;
  }
  else if( queryMsg.callbackQueryData == CB_COLOR_ORANGE ) {
    newMessage = "The color of the light is set to orange";
    lightStatus.color = lcOrange;
  }
  else if( queryMsg.callbackQueryData == CB_COLOR_RED ) {
    newMessage = "The color of the light is set to red";
    lightStatus.color = lcRed;
  }
  else if( queryMsg.callbackQueryData == CB_TIME_3MIN ) {
    newMessage = "Timer set to 3 minutes";
    lightStatus.timer = lt03;
  }
  else if( queryMsg.callbackQueryData == CB_TIME_5MIN ) {
    newMessage = "Timer set to 5 minutes";
    lightStatus.timer = lt05;
  }
  else if( queryMsg.callbackQueryData == CB_TIME_10MIN ) {
    newMessage = "Timer set to 10 minutes";
    lightStatus.timer = lt10;
  }
  else if( queryMsg.callbackQueryData == CB_TIME_20MIN ) {
    newMessage = "Timer set to 20 minutes";
    lightStatus.timer = lt20;
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_15 ) {
    newMessage = "Brightness of the light set to 15%";
    lightStatus.brightness = lb15;
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_30 ) {
    newMessage = "Brightness of the light set to 30%";
    lightStatus.brightness = lb30;
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_50 ) {
    newMessage = "Brightness of the light set to 50%";
    lightStatus.brightness = lb50;
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_100 ) {
    newMessage = "Brightness of the light set to 100%";
    lightStatus.brightness = lb100;
  }
  else if( queryMsg.callbackQueryData == CB_LIGHT_ON ) {
    newMessage = "Light switched on. Timer set to " + TIMES_STR[ lightStatus.timer ];
    lightStatus.LightOn();
  }
  else if( queryMsg.callbackQueryData == CB_LIGHT_OFF ) {
    newMessage = "Light switched off";
    lightStatus.LightOff();
  }
  else if( queryMsg.callbackQueryData == CB_SETTINGS ) {
    newMessage = "Settings menu";
    lightStatus.lightOn = false;
    lightStatus.menu = kbSetings;
  }
  else newMessage = "Command not recognized";

  newMessage += "\n" + StatusMessage();

  lightStatus.UpdateStatus();
  lightStatus.settingsChanged = true;
  myBot.editMessage(queryMsg.chatId, queryMsg.messageID, newMessage, *KEYBOARDS[lightStatus.menu] );
  Serial.println( newMessage );
}

void onQueryScreenBrightness(const TBMessage &queryMsg){
  String newMessage;
  
  if( queryMsg.callbackQueryData == CB_SCREEN_BR_1 ) {
    lightStatus.screenBrightness = sb1;
    newMessage = "Screen brightness set to 1/5";
  }
  else if( queryMsg.callbackQueryData == CB_SCREEN_BR_2 ) {
    lightStatus.screenBrightness = sb2;
    newMessage = "Screen brightness set to 2/5";
  }
  else if( queryMsg.callbackQueryData == CB_SCREEN_BR_3 ) {
    lightStatus.screenBrightness = sb3;
    newMessage = "Screen brightness set to 3/5";
  }
  else if( queryMsg.callbackQueryData == CB_SCREEN_BR_4 ) {
    lightStatus.screenBrightness = sb4;
    newMessage = "Screen brightness set to 4/5";
  }
  else if( queryMsg.callbackQueryData == CB_SCREEN_BR_5 ) {
    lightStatus.screenBrightness = sb5;
    newMessage = "Screen brightness set to 5/5";
  }
  else if( queryMsg.callbackQueryData == CB_MAIN_MENU ) {
    newMessage = "Back to main menu";
  }
  else newMessage = "Command not recognized";
  
  setScreenBrightness(lightStatus.screenBrightness);
  lightStatus.menu = kbMain;
  myBot.editMessage(queryMsg.chatId, queryMsg.messageID, newMessage, *KEYBOARDS[lightStatus.menu] );
  Serial.println( newMessage );
}; // onQueryScreenBrightness

void setup() {
  // initialize the Serial
  Serial.begin(115200);

  // Start the filesystem 
  SPIFFS.begin();
  lightStatus.loadSettings(SPIFFS, SETTINGS_FILE);

  // Initialize light
  Serial.println( "Initialize light" );
  strip.begin();
  strip.setBrightness(255);    

  // Initialize display
  Serial.println( "Initialize display" );
  u8g2.initDisplay();
  delay(500);
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_cu12_tr);    
  u8g2.setFontPosCenter();
  u8g2.setCursor( 12, 16);
  u8g2.print( VERSION );    
  u8g2.sendBuffer();

  // Initialize Wifi
  Serial.println( "Initialize WiFi" );
  WiFi.mode(WIFI_STA);

  // Add wifi access points 
  for (const auto &ap : ACCESS_POINTS ) {
    Serial.printf( "%s %s\n", ap.first.c_str(), ap.second.c_str() );
    wifiMulti.addAP( ap.first.c_str(), ap.second.c_str() );
  }

  Serial.println("Connecting Wifi...");
  if(wifiMulti.run( CONNECT_TIMEOUT_MS ) == WL_CONNECTED) {
    Serial.printf( "WiFi connected to SSID %s signl strength %ddB\n", WiFi.SSID().c_str(), WiFi.RSSI() );
  }
  else {
    Serial.println("WiFi not connected yet");
  }

  WiFi.setAutoReconnect(true);

  // Sync time with NTP
  Serial.println("Sync clock with timeserver");
  configTzTime(localTimezone, "time.google.com", "time.windows.com", "pool.ntp.org");
  
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
  String btntext;
  
  btntext=String(EMOTICON_LIGHT_ON)   + " On";       mainKeyboard.addButton(btntext.c_str(), CB_LIGHT_ON,       KeyboardButtonQuery, onQueryReply);
  btntext=String(EMOTICON_LIGHT_OFF)  + " Off";      mainKeyboard.addButton(btntext.c_str(), CB_LIGHT_OFF,      KeyboardButtonQuery, onQueryReply);
  mainKeyboard.addRow();
  btntext=String(EMOTICON_WHITE)      + " White";    mainKeyboard.addButton(btntext.c_str(), CB_COLOR_WHITE,    KeyboardButtonQuery, onQueryReply);
  btntext=String(EMOTICON_TIMER)      + " 3 min";    mainKeyboard.addButton(btntext.c_str(), CB_TIME_3MIN,      KeyboardButtonQuery, onQueryReply);
  btntext=String(EMOTICON_BRIGHTNESS) + " 15%";      mainKeyboard.addButton(btntext.c_str(), CB_BRIGHTNESS_15,  KeyboardButtonQuery, onQueryReply);
  mainKeyboard.addRow();
  btntext=String(EMOTICON_YELLOW)     + " Yellow";   mainKeyboard.addButton(btntext.c_str(), CB_COLOR_YELLOW,   KeyboardButtonQuery, onQueryReply);
  btntext=String(EMOTICON_TIMER)      + " 5 min";    mainKeyboard.addButton(btntext.c_str(), CB_TIME_5MIN,      KeyboardButtonQuery, onQueryReply);
  btntext=String(EMOTICON_BRIGHTNESS) + " 30%";      mainKeyboard.addButton(btntext.c_str(), CB_BRIGHTNESS_30,  KeyboardButtonQuery, onQueryReply);
  mainKeyboard.addRow();
  btntext=String(EMOTICON_ORANGE)     + " Orange";   mainKeyboard.addButton(btntext.c_str(), CB_COLOR_ORANGE,   KeyboardButtonQuery, onQueryReply);
  btntext=String(EMOTICON_TIMER)      + " 10 min";   mainKeyboard.addButton(btntext.c_str(), CB_TIME_10MIN,     KeyboardButtonQuery, onQueryReply);
  btntext=String(EMOTICON_BRIGHTNESS) + " 50%";      mainKeyboard.addButton(btntext.c_str(), CB_BRIGHTNESS_50,  KeyboardButtonQuery, onQueryReply);
  mainKeyboard.addRow();
  btntext=String(EMOTICON_RED)        + " Red";      mainKeyboard.addButton(btntext.c_str(), CB_COLOR_RED,      KeyboardButtonQuery, onQueryReply);
  btntext=String(EMOTICON_TIMER)      + " 20 min";   mainKeyboard.addButton(btntext.c_str(), CB_TIME_20MIN,     KeyboardButtonQuery, onQueryReply);
  btntext=String(EMOTICON_BRIGHTNESS) + " 100%";     mainKeyboard.addButton(btntext.c_str(), CB_BRIGHTNESS_100, KeyboardButtonQuery, onQueryReply);
  mainKeyboard.addRow();
  btntext=String(EMOTICON_SETTINGS)   + " Settings"; mainKeyboard.addButton(btntext.c_str(), CB_SETTINGS,       KeyboardButtonQuery, onQueryReply);

  btntext=String(EMOTICON_BRIGHTNESS) + " Screen brightness 1"; settingsKeyboard.addButton(btntext.c_str(), CB_SCREEN_BR_1, KeyboardButtonQuery, onQueryScreenBrightness);
  settingsKeyboard.addRow();
  btntext=String(EMOTICON_BRIGHTNESS) + " Screen brightness 2"; settingsKeyboard.addButton(btntext.c_str(), CB_SCREEN_BR_2, KeyboardButtonQuery, onQueryScreenBrightness);
  settingsKeyboard.addRow();
  btntext=String(EMOTICON_BRIGHTNESS) + " Screen brightness 3"; settingsKeyboard.addButton(btntext.c_str(), CB_SCREEN_BR_3, KeyboardButtonQuery, onQueryScreenBrightness);
  settingsKeyboard.addRow();
  btntext=String(EMOTICON_BRIGHTNESS) + " Screen brightness 4"; settingsKeyboard.addButton(btntext.c_str(), CB_SCREEN_BR_4, KeyboardButtonQuery, onQueryScreenBrightness);
  settingsKeyboard.addRow();
  btntext=String(EMOTICON_BRIGHTNESS) + " Screen brightness 5"; settingsKeyboard.addButton(btntext.c_str(), CB_SCREEN_BR_5, KeyboardButtonQuery, onQueryScreenBrightness);
  settingsKeyboard.addRow();
  btntext=String(EMOTICON_MAIN_MENU)  + " Back to main menu"; settingsKeyboard.addButton(btntext.c_str(), CB_MAIN_MENU,   KeyboardButtonQuery, onQueryScreenBrightness);

  // Add pointer to this keyboard to bot (in order to run callback function)
  myBot.addInlineKeyboard(&mainKeyboard);
  myBot.addInlineKeyboard(&settingsKeyboard);

  String text = String(EMOTICON_WELCOME) + " Welcome";
  myBot.sendTo(userid, text.c_str(), mainKeyboard.getJSON() );

  Serial.println("End of setup loop");
}


void loop() {
  static bool wifiConnectReported = false;
  static bool wifiNotConnectReported = false;

  // Check if WiFi is still alive
  if (wifiMulti.run( CONNECT_TIMEOUT_MS ) == WL_CONNECTED) {  /*if the connection lost it will connect to next network*/
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

  // Check if button is pressed
  static volatile uint32_t keyLeftCounter  = 0;
  static volatile uint32_t keyRightCounter = 0;
  static volatile uint32_t keyTopCounter   = 0;

  if (touchRead(PIN_KEY_LEFT )<KEY_TRESHOLD) keyLeftCounter++;  else keyLeftCounter =0; 
  if (touchRead(PIN_KEY_TOP  )<KEY_TRESHOLD) keyTopCounter++;   else keyTopCounter  =0; 
  if (touchRead(PIN_KEY_RIGHT)<KEY_TRESHOLD) keyRightCounter++; else keyRightCounter=0; 

  static unsigned long screenChange = millis();

  if(keyLeftCounter ==2) { /* Nothing to do yet */ };
  if(keyTopCounter  ==2) { lightStatus.SwitchLight();  };
  if(keyRightCounter==2) { lightStatus.screen = scnWeather; screenChange = millis(); };
  
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
  if ( lightStatus.lightOn and (currentTime - lightStatus.previousTimer >= TIMES[lightStatus.timer] ) ) {
    lightStatus.lightOn = false;
    lightStatus.UpdateStatus();
  };

  // Switch back to main screen after time has passed
  const unsigned long mainScreenInterval = 5*1000;
  if (currentTime - screenChange >= mainScreenInterval ) {
    lightStatus.screen = scnMain;
  };

  // Periodically get new weather
  static bool firstTimeWeather = true;
  const unsigned long getWeatherInterval = 10*60*1000;
  //const unsigned long getWeatherInterval = 5*1000;
  static unsigned long previousGetWeather = millis();

  if ( firstTimeWeather or (currentTime - previousGetWeather >= getWeatherInterval ) ) {
    Serial.print("Getting the weather...");
    if( getWeather(weather) ) {
      previousGetWeather = currentTime;
      firstTimeWeather = false;
      Serial.println("success");
    } else {
      Serial.println("fail");
    }
  };

  switch( lightStatus.screen ) {
    case scnMain:
      // Update clock
      time_t rawtime;
      struct tm * timeinfo;
      static int prev_min = -1;
      static bool firstClockUpdate = true;
      
      time( &rawtime );
      timeinfo = localtime( &rawtime );
      if( firstClockUpdate or ( ( timeinfo->tm_year > 80 ) and ( timeinfo->tm_min != prev_min ) ) ) {
        prev_min = timeinfo->tm_min;
        firstClockUpdate = false;

        u8g2.clearBuffer();

        char timestr[12];
        snprintf(timestr, 12, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min );
        Serial.printf("Update time %s\n", timestr);
        
        //u8g2.setFont(u8g2_font_missingplanet_tr);
        //u8g2.setFont(u8g2_font_calibration_gothic_nbp_tr);
        u8g2.setFont(u8g2_font_courR18_tr);
        //u8g2.setFont(u8g2_font_helvR10_tf); // Includes degree symbol
        
        u8g2.setFontPosCenter();
        u8g2.setCursor( ( (u8g2.getDisplayWidth() - (u8g2.getUTF8Width(timestr))) / 2) , 16);
        u8g2.print( timestr );    
    
        u8g2.sendBuffer();
      }
    break;

    case scnWeather:
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_helvR14_tf); // Includes degree symbol
  
      if( weather.updated ) {
        String temperature = String(weather.outsideTemp, 1) + DEGREE_SYMBOL + "C";
        u8g2.setFontPosCenter();
        u8g2.setCursor( ( (u8g2.getDisplayWidth() - (u8g2.getUTF8Width( temperature.c_str() ))) / 2) , 16);
        u8g2.print( temperature.c_str() );    
        u8g2.sendBuffer();
      }
    break;
  }

  // Periodically save settings
  // Increase or decrease light intensity in small timesteps
  const unsigned long saveSettingsInterval = 10*60*1000;
  static unsigned long previousSaveSettings = millis();

  if (currentTime - previousSaveSettings >= saveSettingsInterval ) {
    previousSaveSettings = currentTime;
    if( lightStatus.settingsChanged) lightStatus.saveSettings(SPIFFS, SETTINGS_FILE, SETTINGS_TEMP);
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
          myBot.sendMessage(msg, "Welcome!", mainKeyboard);      
          Serial.printf("Start command received from %d", msg.chatId);    
        }        
        else {
          // write back feedback message and show a hint
          String text = String("const char EMOTICON[] = ") + convertToHexString( msg.text );
          myBot.sendMessage(msg, text.c_str(), mainKeyboard);
        }
        break;
        
        case MessageQuery:
          // Handled by message query callback functions
          break;
        
        default:
          break;
    }
  }
}
