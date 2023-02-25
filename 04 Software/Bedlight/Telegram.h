#pragma once

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <AsyncTelegram2.h>
#include <map>

#include "Data.h"
#include "Light.h"

const uint32_t CONNECT_TIMEOUT_MS = 10000;

// Messages for the callback functions
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
const char EMOTICON_RAINBOW[]     = { 0xf0, 0x9f, 0x8c, 0x88, 0x00 };
const char EMOTICON_TIMER[]       = { 0xe2, 0x8f, 0xb2, 0x00 };
const char EMOTICON_BRIGHTNESS[]  = { 0xf0, 0x9f, 0x94, 0x86, 0x00 };
const char EMOTICON_SETTINGS[]    = { 0xe2, 0x9a, 0x99, 0xef, 0xb8, 0x8f, 0x00 };
const char EMOTICON_LIGHT_ON[]    = { 0xf0, 0x9f, 0x92, 0xa1, 0x00 };
const char EMOTICON_LIGHT_OFF[]   = { 0xf0, 0x9f, 0x98, 0xb4, 0x00 };
const char EMOTICON_MAIN_MENU[]   = { 0xf0, 0x9f, 0x8f, 0xa0, 0x00 };

const char DEGREE_SYMBOL[]        = { 0xB0, '\0' };


// ======== GLOBAL VARIABLES ============= 
WiFiMulti wifiMulti;
WiFiClientSecure client;  
AsyncTelegram2 myBot(client);
InlineKeyboard mainKeyboard, settingsKeyboard;

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
  result = String(EMOTICON_LIGHT_ON) + String( data.lightOn ? " On " : " Off " );
  switch( data.color ) {
    case lcWhite:  result+= EMOTICON_WHITE;  break;
    case lcYellow: result+= EMOTICON_YELLOW; break;
    case lcOrange: result+= EMOTICON_ORANGE; break;
    case lcRed:    result+= EMOTICON_RED;    break;
  }

   
  result += String(" ") + EMOTICON_BRIGHTNESS + " " + String(100*BRIGHTNESSES[data.brightness],0)+"%";
  result += String(" ") + EMOTICON_TIMER + " " + TIMES_STR[data.timer];
  return result;
};  
  
// Callback functions definition for inline keyboard buttons
void onQueryReply(const TBMessage &queryMsg){

  String newMessage = "Command not recognized";

  if( queryMsg.callbackQueryData == CB_COLOR_WHITE ) {
    newMessage = "The color of the light is set to white";
    data.color = lcWhite;
    data.settingsChanged = true;
  }
  else if( queryMsg.callbackQueryData == CB_COLOR_YELLOW ) {
    newMessage = "The color of the light is set to yellow";
    data.color = lcYellow;
    data.settingsChanged = true;
  }
  else if( queryMsg.callbackQueryData == CB_COLOR_ORANGE ) {
    newMessage = "The color of the light is set to orange";
    data.color = lcOrange;
    data.settingsChanged = true;
  }
  else if( queryMsg.callbackQueryData == CB_COLOR_RED ) {
    newMessage = "The color of the light is set to red";
    data.color = lcRed;
    data.settingsChanged = true;
  }
  else if( queryMsg.callbackQueryData == CB_TIME_3MIN ) {
    newMessage = "Timer set to 3 minutes";
    data.timer = lt03;
    data.settingsChanged = true;
  }
  else if( queryMsg.callbackQueryData == CB_TIME_5MIN ) {
    newMessage = "Timer set to 5 minutes";
    data.timer = lt05;
    data.settingsChanged = true;
  }
  else if( queryMsg.callbackQueryData == CB_TIME_10MIN ) {
    newMessage = "Timer set to 10 minutes";
    data.timer = lt10;
    data.settingsChanged = true;
  }
  else if( queryMsg.callbackQueryData == CB_TIME_20MIN ) {
    newMessage = "Timer set to 20 minutes";
    data.timer = lt20;
    data.settingsChanged = true;
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_15 ) {
    newMessage = "Brightness of the light set to 15%";
    data.brightness = lb15;
    data.settingsChanged = true;
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_30 ) {
    newMessage = "Brightness of the light set to 30%";
    data.brightness = lb30;
    data.settingsChanged = true;
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_50 ) {
    newMessage = "Brightness of the light set to 50%";
    data.brightness = lb50;
    data.settingsChanged = true;
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_100 ) {
    newMessage = "Brightness of the light set to 100%";
    data.brightness = lb100;
    data.settingsChanged = true;
  }
  else if( queryMsg.callbackQueryData == CB_LIGHT_ON ) {
    newMessage = "Light switched on. Timer set to " + TIMES_STR[ data.timer ];
    data.LightOn();
  }
  else if( queryMsg.callbackQueryData == CB_LIGHT_OFF ) {
    newMessage = "Light switched off";
    data.LightOff();
  }
  else if( queryMsg.callbackQueryData == CB_SETTINGS ) {
    newMessage = "Settings menu";
    data.lightOn = false;
    data.menu = kbSetings;
  }
  else newMessage = "Command not recognized";

  newMessage += "\n" + StatusMessage();

  data.UpdateStatus();
  data.settingsChanged = true;
  myBot.editMessage(queryMsg.chatId, queryMsg.messageID, newMessage, *KEYBOARDS[data.menu] );
  Serial.println( newMessage );
}

void onQueryScreenBrightness(const TBMessage &queryMsg){
  String newMessage;
  
  if( queryMsg.callbackQueryData == CB_SCREEN_BR_1 ) {
    data.screenBrightness = sb1;
    data.settingsChanged = true;
    newMessage = "Screen brightness set to 1/5";
  }
  else if( queryMsg.callbackQueryData == CB_SCREEN_BR_2 ) {
    data.screenBrightness = sb2;
    data.settingsChanged = true;
    newMessage = "Screen brightness set to 2/5";
  }
  else if( queryMsg.callbackQueryData == CB_SCREEN_BR_3 ) {
    data.screenBrightness = sb3;
    data.settingsChanged = true;
    newMessage = "Screen brightness set to 3/5";
  }
  else if( queryMsg.callbackQueryData == CB_SCREEN_BR_4 ) {
    data.screenBrightness = sb4;
    data.settingsChanged = true;
    newMessage = "Screen brightness set to 4/5";
  }
  else if( queryMsg.callbackQueryData == CB_SCREEN_BR_5 ) {
    data.screenBrightness = sb5;
    data.settingsChanged = true;
    newMessage = "Screen brightness set to 5/5";
  }
  else if( queryMsg.callbackQueryData == CB_MAIN_MENU ) {
    newMessage = "Back to main menu";
  }
  else newMessage = "Command not recognized";

  newMessage += "\n" + StatusMessage();
  
  data.menu = kbMain;
  myBot.editMessage(queryMsg.chatId, queryMsg.messageID, newMessage, *KEYBOARDS[data.menu] );
  Serial.println( newMessage );
}; // onQueryScreenBrightness

void addInlineKeyboard() {
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
}

void setupWifi() {
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
};

void setupTelegram() {
  
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

  addInlineKeyboard();
  
  // Add pointer to this keyboard to bot (in order to run callback function)
  myBot.addInlineKeyboard(&mainKeyboard);
  myBot.addInlineKeyboard(&settingsKeyboard);

  String text = String(EMOTICON_WELCOME) + " Welcome";
  myBot.sendTo(userid, text.c_str(), mainKeyboard.getJSON() );

  Serial.println("End of setup loop");
};


void loopTelegram() {
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
