#pragma once

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <AsyncTelegram2.h>
#include <map>

#include "a_Data.h"         // This file contains all types and the struct 'data' which acts as a central databus
#include "c_Log.h"          // Logfile for weather data
#include "d_Eventlog.h"     // Event loggers
#include "e_Light.h"        // Controlling the light

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
#define CB_DEBUG_INFO      "deb_info"
#define CB_EVT_SEND        "event_send" 
#define CB_EVT_CLEAR       "event_clear" 
#define CB_LOG_SEND        "log_send" 
#define CB_LOG_CLEAR       "log_clear" 
#define CB_MAIN_MENU       "mainmenu" 
#define CB_MN_SCR_BR       "mnuScreen"
#define CB_SCREEN_BR_1     "scnbrht1" 
#define CB_SCREEN_BR_2     "scnbrht2" 
#define CB_SCREEN_BR_3     "scnbrht3" 
#define CB_SCREEN_BR_4     "scnbrht4" 
#define CB_SCREEN_BR_5     "scnbrht5" 

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

const unsigned char DEGREE_SYMBOL[]        = { 0xB0, '\0' };

// ======== GLOBAL VARIABLES ============= 
WiFiClientSecure client;  
AsyncTelegram2 myBot(client);
InlineKeyboard mainKeyboard, settingsKeyboard, screenBrightnessKeyboard, logKeyboard;

std::map<keyboard_t, InlineKeyboard* > KEYBOARDS = { 
  { kbMain,             &mainKeyboard             },
  { kbSettings,         &settingsKeyboard         },
  { kbScreenBrightness, &screenBrightnessKeyboard }, 
  { kbLog,              &logKeyboard              }
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

void sendMessageToKeyUser( String msg ) {
  myBot.sendTo(userid, msg.c_str(), mainKeyboard.getJSON() );  
}

String StatusMessage() {
  String result;
  result = String(EMOTICON_LIGHT_ON) + String( data.lightOn ? " On " : " Off " );
  switch( data.color ) {
    case lcWhite:  result+= EMOTICON_WHITE;  break;
    case lcYellow: result+= EMOTICON_YELLOW; break;
    case lcOrange: result+= EMOTICON_ORANGE; break;
    case lcRed:    result+= EMOTICON_RED;    break;
  } // switch( data.color )

  result += String(" ") + EMOTICON_BRIGHTNESS + " " + String(100*BRIGHTNESSES[data.brightness],0)+"%";
  result += String(" ") + EMOTICON_TIMER + " " + TIMES_STR[data.timer];
  return result;
};  
  
// Callback functions definition for inline keyboard buttons
void onQueryMain( const TBMessage &queryMsg ) {
  String newMessage = "Command not recognized";

  if( queryMsg.callbackQueryData == CB_COLOR_WHITE ) {
    newMessage = "The color of the light is set to white";
    data.setColor(lcWhite);
  }
  else if( queryMsg.callbackQueryData == CB_COLOR_YELLOW ) {
    newMessage = "The color of the light is set to yellow";
    data.setColor(lcYellow);
  }
  else if( queryMsg.callbackQueryData == CB_COLOR_ORANGE ) {
    newMessage = "The color of the light is set to orange";
    data.setColor(lcOrange);
  }
  else if( queryMsg.callbackQueryData == CB_COLOR_RED ) {
    newMessage = "The color of the light is set to red";
    data.setColor(lcRed);
  }
  else if( queryMsg.callbackQueryData == CB_TIME_3MIN ) {
    newMessage = "Timer set to 3 minutes";
    data.setTimer( lt03 );
  }
  else if( queryMsg.callbackQueryData == CB_TIME_5MIN ) {
    newMessage = "Timer set to 5 minutes";
    data.setTimer( lt05 );
  }
  else if( queryMsg.callbackQueryData == CB_TIME_10MIN ) {
    newMessage = "Timer set to 10 minutes";
    data.setTimer( lt10 );
  }
  else if( queryMsg.callbackQueryData == CB_TIME_20MIN ) {
    newMessage = "Timer set to 20 minutes";
    data.setTimer( lt20 );
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_15 ) {
    newMessage = "Brightness of the light set to 15%";
    data.setBrightness( lb15 );
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_30 ) {
    newMessage = "Brightness of the light set to 30%";
    data.setBrightness( lb30 );
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_50 ) {
    newMessage = "Brightness of the light set to 50%";
    data.setBrightness( lb50 );
  }
  else if( queryMsg.callbackQueryData == CB_BRIGHTNESS_100 ) {
    newMessage = "Brightness of the light set to 100%";
    data.setBrightness( lb100 );
  }
  else if( queryMsg.callbackQueryData == CB_LIGHT_ON ) {
    newMessage = "Light switched on. Timer set to " + TIMES_STR[ data.timer ];
    data.setLightOn();
  }
  else if( queryMsg.callbackQueryData == CB_LIGHT_OFF ) {
    newMessage = "Light switched off";
    data.setLightOff();
  }
  else if( queryMsg.callbackQueryData == CB_SETTINGS ) {
    newMessage = "Settings menu";
    data.menu = kbSettings;
  }
  else newMessage = "Command not recognized";

  newMessage += "\n" + StatusMessage();

  myBot.editMessage(queryMsg.chatId, queryMsg.messageID, newMessage, *KEYBOARDS[data.menu] );
  Serial.println( newMessage );
}

void onQuerySettings(const TBMessage &queryMsg) {
  String newMessage;
  char item[80];
  struct tm * timeinfo;

  if( queryMsg.callbackQueryData == CB_MN_SCR_BR ) {
    newMessage = "Screen brightness menu";
    data.menu = kbScreenBrightness;
  }
  else if( queryMsg.callbackQueryData == CB_DEBUG_INFO ) {

    // First item must not use +=
    newMessage = "Outside temperature: " + String( data.outsideTemp ) + "°C\n";
    newMessage += "Feels like temp: " + String( data.outsideFeelsLike ) + "°C\n";
    newMessage += "Max day temp: " + String( data.maxDayTemp ) + "°C\n";
    newMessage += "Max tomorrow temp: " + String( data.maxTomorrowTemp ) + "°C\n";
    newMessage += "Max temp displayed: " + String( data.maxDisplayTemp ) + "°C\n";

    timeinfo = localtime ( &(data.daySunrise ) );
    snprintf(item, sizeof( item ), "Sunrise %d:%02d\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
    newMessage += item;

    timeinfo = localtime ( &(data.daySunset ) );
    snprintf(item, sizeof( item ), "Sunset %d:%02d\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
    newMessage += item;

    newMessage += "Air pressure: " + String( data.pressure ) + " mbar\n";
    newMessage += "Humidity: " + String( data.humidity ) + "%\n";
    newMessage += "Wind speed: " + String( data.windspeed ) + " m/s\n";
    newMessage += "Wind direction: " + String( data.windDirection ) + "° " + data.windDirStr + "\n";
  
    newMessage += "Weather retrieval counter: " + String( (int) data.weatherRetrievalCounter ) + "\n";

    // Last item must not get \n 
    timeinfo = localtime ( &(data.lastWeatherUpdate) );
    snprintf(item, sizeof( item ), "Last weather update %02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
    newMessage += item;
  }  
  else if( queryMsg.callbackQueryData == CB_EVT_SEND ) {
    newMessage = "Send event log";
    File file = SPIFFS.open(EVT_LOG_FILE, "r");
    if (file) {
      myBot.sendDocument(queryMsg, file, file.size(), AsyncTelegram2::DocumentType::CSV, file.name());
      file.close();
    }
    else
      Serial.println("Can't open the event log file");
  }  
  else if( queryMsg.callbackQueryData == CB_EVT_CLEAR ) {
    newMessage = "Clear event log";
    newEventLogfile();
  }  
  else if( queryMsg.callbackQueryData == CB_LOG_SEND ) {
    newMessage = "Send logfile";
    File file = SPIFFS.open(LOG_FILE, "r");
    if (file) {
      myBot.sendDocument(queryMsg, file, file.size(), AsyncTelegram2::DocumentType::CSV, file.name());
      file.close();
    }
    else
      Serial.println("Can't open the log file");
  }  
  else if( queryMsg.callbackQueryData == CB_LOG_CLEAR ) {
    newMessage = "Clear log file";
    newLogfile();
  }  
  else if( queryMsg.callbackQueryData == CB_MN_SCR_BR ) {
    newMessage = "Set screen brightness";
    data.menu = kbScreenBrightness;
  }  
  else if( queryMsg.callbackQueryData == CB_MAIN_MENU ) {
    newMessage = "Back to main menu";
    data.menu = kbMain;
  }
  else newMessage = "Command not recognized";

  newMessage += "\n" + StatusMessage();  
  myBot.editMessage( queryMsg.chatId, queryMsg.messageID, newMessage, *KEYBOARDS[ data.menu ] );
  Serial.println( newMessage );
}; // onQuerySettings


void onQueryScreenBrightness(const TBMessage &queryMsg) {
  String newMessage;
  
  if( queryMsg.callbackQueryData == CB_SCREEN_BR_1 ) {
    newMessage = "Screen brightness set to 1/5";
    data.setScreenBrightness( sb1 );
  }
  else if( queryMsg.callbackQueryData == CB_SCREEN_BR_2 ) {
    newMessage = "Screen brightness set to 2/5";
    data.setScreenBrightness( sb2 );
  }
  else if( queryMsg.callbackQueryData == CB_SCREEN_BR_3 ) {
    newMessage = "Screen brightness set to 3/5";
    data.setScreenBrightness( sb3 );
  }
  else if( queryMsg.callbackQueryData == CB_SCREEN_BR_4 ) {
    newMessage = "Screen brightness set to 4/5";
    data.setScreenBrightness( sb4 );
  }
  else if( queryMsg.callbackQueryData == CB_SCREEN_BR_5 ) {
    newMessage = "Screen brightness set to 5/5";
    data.setScreenBrightness( sb5 );
  }
  else if( queryMsg.callbackQueryData == CB_SETTINGS ) {
    newMessage = "Back to settings menu";
  }
  else newMessage = "Command not recognized";

  newMessage += "\n" + StatusMessage();
  
  data.menu = kbSettings;
  myBot.editMessage( queryMsg.chatId, queryMsg.messageID, newMessage, *KEYBOARDS[ data.menu ] );
  Serial.println( newMessage );
}; // onQueryScreenBrightness


void addInlineKeyboard() {
  // Add sample inline keyboard
  String btntext;
  
  // Add buttons for main keyboard
  btntext=String(EMOTICON_LIGHT_ON)   + " On";       
  mainKeyboard.addButton(btntext.c_str(), CB_LIGHT_ON,       KeyboardButtonQuery, onQueryMain);
  btntext=String(EMOTICON_LIGHT_OFF)  + " Off";      
  mainKeyboard.addButton(btntext.c_str(), CB_LIGHT_OFF,      KeyboardButtonQuery, onQueryMain);
  mainKeyboard.addRow();
  
  btntext=String(EMOTICON_WHITE)      + " White";    
  mainKeyboard.addButton(btntext.c_str(), CB_COLOR_WHITE,    KeyboardButtonQuery, onQueryMain);
  btntext=String(EMOTICON_TIMER)      + " 3 min";    
  mainKeyboard.addButton(btntext.c_str(), CB_TIME_3MIN,      KeyboardButtonQuery, onQueryMain);
  btntext=String(EMOTICON_BRIGHTNESS) + " 15%";      
  mainKeyboard.addButton(btntext.c_str(), CB_BRIGHTNESS_15,  KeyboardButtonQuery, onQueryMain);
  mainKeyboard.addRow();

  btntext=String(EMOTICON_YELLOW)     + " Yellow";   
  mainKeyboard.addButton(btntext.c_str(), CB_COLOR_YELLOW,   KeyboardButtonQuery, onQueryMain);
  btntext=String(EMOTICON_TIMER)      + " 5 min";    
  mainKeyboard.addButton(btntext.c_str(), CB_TIME_5MIN,      KeyboardButtonQuery, onQueryMain);
  btntext=String(EMOTICON_BRIGHTNESS) + " 30%";      
  mainKeyboard.addButton(btntext.c_str(), CB_BRIGHTNESS_30,  KeyboardButtonQuery, onQueryMain);
  mainKeyboard.addRow();

  btntext=String(EMOTICON_ORANGE)     + " Orange";   
  mainKeyboard.addButton(btntext.c_str(), CB_COLOR_ORANGE,   KeyboardButtonQuery, onQueryMain);
  btntext=String(EMOTICON_TIMER)      + " 10 min";   
  mainKeyboard.addButton(btntext.c_str(), CB_TIME_10MIN,     KeyboardButtonQuery, onQueryMain);
  btntext=String(EMOTICON_BRIGHTNESS) + " 50%";      
  mainKeyboard.addButton(btntext.c_str(), CB_BRIGHTNESS_50,  KeyboardButtonQuery, onQueryMain);
  mainKeyboard.addRow();

  btntext=String(EMOTICON_RED)        + " Red";      
  mainKeyboard.addButton(btntext.c_str(), CB_COLOR_RED,      KeyboardButtonQuery, onQueryMain);
  btntext=String(EMOTICON_TIMER)      + " 20 min";   
  mainKeyboard.addButton(btntext.c_str(), CB_TIME_20MIN,     KeyboardButtonQuery, onQueryMain);
  btntext=String(EMOTICON_BRIGHTNESS) + " 100%";     
  mainKeyboard.addButton(btntext.c_str(), CB_BRIGHTNESS_100, KeyboardButtonQuery, onQueryMain);
  mainKeyboard.addRow();

  btntext=String(EMOTICON_SETTINGS)   + " Settings"; 
  mainKeyboard.addButton(btntext.c_str(), CB_SETTINGS,       KeyboardButtonQuery, onQueryMain);

  // Add buttons for settings keyboard
  settingsKeyboard.addButton("Set screen brightness",   CB_MN_SCR_BR , KeyboardButtonQuery, onQuerySettings);
  settingsKeyboard.addRow();

  settingsKeyboard.addButton("Download event log",  CB_EVT_SEND , KeyboardButtonQuery, onQuerySettings);
  settingsKeyboard.addButton("Clear event log", CB_EVT_CLEAR, KeyboardButtonQuery, onQuerySettings);
  settingsKeyboard.addRow();

  settingsKeyboard.addButton("Download logfile",  CB_LOG_SEND , KeyboardButtonQuery, onQuerySettings);
  settingsKeyboard.addButton("Clear logfile", CB_LOG_CLEAR, KeyboardButtonQuery, onQuerySettings);
  settingsKeyboard.addRow();

  settingsKeyboard.addButton("Debug info", CB_DEBUG_INFO , KeyboardButtonQuery, onQuerySettings);
  settingsKeyboard.addRow();

  btntext=String(EMOTICON_MAIN_MENU)  + " Back to main menu"; 
  settingsKeyboard.addButton(btntext.c_str(), CB_MAIN_MENU,   KeyboardButtonQuery, onQuerySettings);

  // Add buttons for settings keyboard
  btntext=String(EMOTICON_BRIGHTNESS) + " Screen brightness 1"; 
  screenBrightnessKeyboard.addButton(btntext.c_str(), CB_SCREEN_BR_1, KeyboardButtonQuery, onQueryScreenBrightness);
  screenBrightnessKeyboard.addRow();
  
  btntext=String(EMOTICON_BRIGHTNESS) + " Screen brightness 2"; 
  screenBrightnessKeyboard.addButton(btntext.c_str(), CB_SCREEN_BR_2, KeyboardButtonQuery, onQueryScreenBrightness);
  screenBrightnessKeyboard.addRow();
  
  btntext=String(EMOTICON_BRIGHTNESS) + " Screen brightness 3"; 
  screenBrightnessKeyboard.addButton(btntext.c_str(), CB_SCREEN_BR_3, KeyboardButtonQuery, onQueryScreenBrightness);
  screenBrightnessKeyboard.addRow();

  btntext=String(EMOTICON_BRIGHTNESS) + " Screen brightness 4"; 
  screenBrightnessKeyboard.addButton(btntext.c_str(), CB_SCREEN_BR_4, KeyboardButtonQuery, onQueryScreenBrightness);
  screenBrightnessKeyboard.addRow();

  btntext=String(EMOTICON_BRIGHTNESS) + " Screen brightness 5"; 
  screenBrightnessKeyboard.addButton(btntext.c_str(), CB_SCREEN_BR_5, KeyboardButtonQuery, onQueryScreenBrightness);
  screenBrightnessKeyboard.addRow();

  btntext=String(EMOTICON_SETTINGS)   + " Back to settings"; 
  screenBrightnessKeyboard.addButton(btntext.c_str(), CB_SETTINGS,    KeyboardButtonQuery, onQueryScreenBrightness);
}

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
  myBot.addInlineKeyboard(&screenBrightnessKeyboard);

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
          Serial.printf("Start command received from %d\n", msg.chatId);    
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
