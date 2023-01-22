#pragma once

#include <map>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <HTTPClient.h>

#include "a_data.h"

#define TELEGRAM_INTERVAL 1000

using namespace std;

/* =================================================================== *
 *  DATA STRUCTURES THAT MANAGE USERS, MENUS AND COMMANDS IN TELEGRAM  *
 * =================================================================== */

// Helper function that generates JSON code for an inline button
String btnInline(command_t command) {
  return "{ \"text\" : \"" + telegramButtons[command].label + "\", \"callback_data\" : \"" + telegramButtons[command].callback+ "\" }";
};

// Helper function that updates button text for certain buttons and generates JSON code for the keyboard of a specific screen
String keyboard(screen_t  & screen ) {
  
  return "[[" + btnInline(cmdLightOn)    + "," + btnInline(cmdLightOff)                                       + "]," +
          "[" + btnInline(cmdDuration03) +  ","+ btnInline(cmdColorWhite)  + ","+ btnInline(cmdBrightness25)  + "]," +
          "[" + btnInline(cmdDuration05) +  ","+ btnInline(cmdColorYellow) + ","+ btnInline(cmdBrightness40)  + "]," +
          "[" + btnInline(cmdDuration10) +  ","+ btnInline(cmdColorAmber)  + ","+ btnInline(cmdBrightness60)  + "]," +
          "[" + btnInline(cmdDuration20) +  ","+ btnInline(cmdColorRed)    + ","+ btnInline(cmdBrightness100) + "]," +
          "[" + btnInline(cmdDurationForever)                                                                 + "]]";
};


// One TelegramChat object for each user known in the system
class TelegramChat {
  public:
    TelegramChat() {}; // An empty contructor is required for the class to be used in a map
    TelegramChat(UniversalTelegramBot & bot, String chatID, data_t & data, QueueHandle_t lightQueue );
    bool lastMessageIDknown = false;
    String chatID;
    int lastMessageID = 0;

    screen_t screen = scnMain;

    // This function recognizes a callback and sends a command to the controller or directly to the Telegram queue
    void handleCallback(String & callback);

    // This function updates the status message of the Telegram chat
    void respondToUser(command_t command);
    
  protected:
    UniversalTelegramBot * bot;
    data_t * data;
    QueueHandle_t lightQueue;
};

TelegramChat::TelegramChat(UniversalTelegramBot & bot, String chatID, data_t & data, QueueHandle_t lightQueue ) { 
  this->bot        = & bot;
  this->chatID     =   chatID;
  this->data       = & data; 
  this->lightQueue =   lightQueue; 
};

void TelegramChat::handleCallback(String & callback) {
  // This is the first response as a message is received.
  // If the command potentially changes controller settings, the command is sent to the controller
  // The controller then modifies the setting and lights up the backlight to display the change to users in the room
  // If the command is not changing any settings (i.e. it flips to a different screen), the command is directly forwarded to the Telegram queue

  // Recognize the response from the inline keyboard
  command_t command = cmdCommandNotRecognized;
  for (auto const& kv : telegramButtons) {
    if( callback==kv.second.callback ) {
      command=kv.first;
      break;
    }
  }

  // Send the command to the light queue
  sendCommandToQueue( command, lightQueue );

  // Respond to the user
  respondToUser(command);
         
}; // handleCallback

void TelegramChat::respondToUser( command_t command ) {
  String response=""; // Message to the user
  
  switch (command)  {

    case cmdLightOn:         response= String(EMOTICON_DOT_YELLOW) + " Light on"; break;
    case cmdLightOff:        response= String(EMOTICON_DOT_BLACK)  + " Light off"; break;      
    case cmdLightToggle:     response= "Light toggled on/off"; break;
    case cmdDuration03:      response= "Timer set to 3 minutes"; break;
    case cmdDuration05:      response= "Timer set to 5 minutes"; break;
    case cmdDuration10:      response= "Timer set to 10 minutes"; break;      
    case cmdDuration20:      response= "Timer set to 20 minutes"; break;
    case cmdDurationForever: response= "Timer switched off"; break;
    case cmdColorWhite:      response= "Light color set to white"; break;
    case cmdColorYellow:     response= "Light color set to yellow"; break;
    case cmdColorAmber:      response= "Light color set to amber"; break;
    case cmdColorRed:        response= "Light color set to red"; break;
    case cmdBrightness25:    response= "Brightness set to 25%"; break;
    case cmdBrightness40:    response= "Brightness set to 40%"; break;
    case cmdBrightness60:    response= "Brightness set to 60%"; break;
    case cmdBrightness100:   response= "Brightness set to 100%"; break;      
    case cmdUpdateSoftware:  response= "Ready for software update for the next 5 minutes"; break;      
    
  } // switch (command) 

  if( !lastMessageIDknown ) {  
    // When sending the first message, the chatID containing the keyboard is not yet known
    bot->sendMessageWithInlineKeyboard(chatID, response, "Markdown", keyboard(screen) );
  } else {
    // Update the last message containing a keyboard
    bot->sendMessageWithInlineKeyboard(chatID, response, "Markdown", keyboard(screen), lastMessageID);
  }

  if( command == cmdUpdateSoftware ) {
    unsigned long updateTimeOut = millis();  
    unsigned long blinkTimeOut;  
    bool ledState;

    //disableKeyboard(); // Writing to SPIFFS must not be interrupted by the capacitive touch keys

    // Wait 5 minutes for over the air software update
    while ( millis() - updateTimeOut < 5*60*1000 ){
      ArduinoOTA.handle();

      if ( millis() - blinkTimeOut > 300 ) {
        blinkTimeOut = millis();
        ledState = !ledState;
        digitalWrite(PIN_ESP32_LED, ledState);
      }  
    };
    digitalWrite(PIN_ESP32_LED, false);

    //enableKeyboard();

    bot->sendMessageWithInlineKeyboard(chatID, "Timeout for over the air software update", "Markdown", keyboard(screen), lastMessageID);
  };
};

// ======== GlOBAL DECLARATIONS ================

WiFiClientSecure securedClient;                  // Secure wifi client
UniversalTelegramBot bot("", securedClient);     // Driver for Telegram, bot token not yet retrieved by ControllerData since SPIFFS not yet up
std::map<String, TelegramChat> userConversation; // The data for each user interacting with Telegram


void setupOTA() {
  
  Serial.println("Setting up over the air updates");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
      //disableKeyboard(); // Writing to SPIFFS must not be interrupted by the capacitive touch keys

    })
    .onEnd([]() {
      //enableKeyboard();
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();    
}

void startTelegram() {
  bot.maxMessageLength = 6000;
  //telegramHandler.begin( );

  setupOTA();

  // Setup secure connection for Telegram
  securedClient.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  bot.updateToken(config.botToken);

  const String commands = F("["
                            "{\"command\":\"start\",   \"description\":\"Welcome message\"}" // no comma on last command                            
                            "]");
  bot.setMyCommands(commands);

  // Add first chatID to list
  TelegramChat newTelegramChat=TelegramChat( bot, config.chatID, data, lightQueue);  
  userConversation[config.chatID]=newTelegramChat;

  // Send first message to the user
  bot.sendMessageWithInlineKeyboard( config.chatID, 
                                      String(EMOTICON_LAMP) + " Bedlight just started", 
                                      "Markdown", 
                                      keyboard( userConversation[config.chatID].screen ) 
                                    );  
}

void handleNewMessages(int numNewMessages){
  String response;
  
  for (int i = 0; i < numNewMessages; i++) {
    String chatID = bot.messages[i].chat_id;
    Serial.printf("Message %s from chatID: %s\n", bot.messages[i].text.c_str(), chatID);

    // If chat is not yet listed, create new chatID
    if (userConversation.find(chatID) == userConversation.end()) {
      TelegramChat newTelegramChat=TelegramChat( bot, chatID, data, lightQueue);  
      userConversation[chatID]=newTelegramChat;
      Serial.printf("Chat added to list of chats\n");
      for (auto const& chat : userConversation) {
        Serial.printf("ID: %s [ID: %s]\n", chat.first.c_str(), chat.second.chatID.c_str() );
      }
    }

    // Update data in conversation
    userConversation[chatID].chatID = chatID; // should no longer be necessary
    userConversation[chatID].lastMessageID = bot.messages[i].message_id;
    userConversation[chatID].lastMessageIDknown = true;

    if(bot.messages[i].type=="message") {
      String message = bot.messages[i].text;
      
      // For convenience, return sample code to convert emoticons to printale strings
      if( message == "/start" ) {
        //userConversation[chatID].lastMessageIDknown = false; // Start a new keyboard
        bot.sendMessageWithInlineKeyboard( chatID, 
                                           String(EMOTICON_LAMP) + " Bedlight just started", 
                                           "Markdown", 
                                           keyboard( userConversation[chatID].screen ) 
                                          );  
        //userConversation[chatID].respondToUser( cmdStartTelegram ); 
      }
      else {
        bot.sendMessage(bot.messages[i].chat_id, String( "const char EMOTICON[] = " ) + convertToHexString( message ), "");
      }
      
    }    
    else if (bot.messages[i].type=="callback_query") {
      userConversation[chatID].handleCallback(bot.messages[i].text);
    }
  } // for i
};

void checkTelegramIfNeeded() {
  static unsigned long lastTimeTelegramPolled = 0;
      
  // Do not poll too often. Telegram would get bored with us
  if ( millis() - lastTimeTelegramPolled > TELEGRAM_INTERVAL ) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeTelegramPolled=millis();
  }  

};
