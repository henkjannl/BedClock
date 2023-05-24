#pragma once

#include <WiFi.h>
#include <WiFiMulti.h>
#include <map>

#include "a_Data.h"
#include "i_Eventlog.h"

const uint32_t CONNECT_TIMEOUT_MS = 10000;

milliSecTimer reconnectTimer1 = milliSecTimer(  1*60*1000, false ); // First reconnect attempt
milliSecTimer reconnectTimer2 = milliSecTimer(  2*60*1000, false ); // Second reconnect attempt
milliSecTimer reconnectTimer3 = milliSecTimer(  5*60*1000, false ); // Third reconnect attempt
milliSecTimer restartTimer    = milliSecTimer( 10*60*1000, false ); // Restart ESP

WiFiMulti wifiMulti;

void setupWifi() {
  // Initialize Wifi
  Serial.println( "Initialize WiFi" );
  WiFi.mode(WIFI_STA);

  // Add wifi access points 
  for (const auto &ap : ACCESS_POINTS ) {
    //Serial.printf( "%s %s\n", ap.first.c_str(), ap.second.c_str() );
    wifiMulti.addAP( ap.first.c_str(), ap.second.c_str() );
  }

  Serial.println("Connecting Wifi...");
  if(wifiMulti.run( CONNECT_TIMEOUT_MS ) == WL_CONNECTED) {
    Serial.printf( "WiFi connected to SSID %s signal strength %ddB\n", WiFi.SSID().c_str(), WiFi.RSSI() );
  }
  else {
    Serial.println("WiFi not connected yet");
  }

  WiFi.setAutoReconnect(true);

  // Sync time with NTP
  Serial.println("Sync clock with timeserver");
  configTzTime(localTimezone, "time.google.com", "time.windows.com", "pool.ntp.org");
};

void loopWifi() {
  static bool wifiConnectReported = false;
  static bool wifiNotConnectReported = false;
  static uint8_t reconnectReport = 0;
  uint8_t reconnectAttempt = 0;
  char item[80];

  // Check if WiFi is still alive
  if ( WiFi.status() == WL_CONNECTED) {  /*if the connection lost it will connect to next network*/

    reconnectReport = 0;
    reconnectTimer1.reset();
    reconnectTimer2.reset();
    reconnectTimer3.reset();
    restartTimer.reset();

    if( !wifiConnectReported ) {
      snprintf( item, sizeof(item), "WiFi connected to SSID %s. Signal strength %ddB", WiFi.SSID().c_str(), WiFi.RSSI() );
      addToEventLogfile( item );
      Serial.println( item );

      wifiConnectReported=true;
      wifiNotConnectReported=false;
    };
  }
  else {
    if( !wifiNotConnectReported ) {
      Serial.println("WiFi not connected!");  /*if all conditions fail print this*/
      addToEventLogfile( "Wifi connection lost" );
      wifiNotConnectReported=true;
      wifiConnectReported=false;
    }
  }

  reconnectAttempt = 0;
  if( reconnectTimer1.lapsed() ) reconnectAttempt = 1;
  if( reconnectTimer2.lapsed() ) reconnectAttempt = 2;
  if( reconnectTimer3.lapsed() ) reconnectAttempt = 3;

  if( reconnectAttempt>0 ) {
    // Try to connect to the strongest available network
    wifiMulti.run( CONNECT_TIMEOUT_MS );

    if( reconnectAttempt > reconnectReport ) {
      snprintf( item, sizeof(item), "%s attempt to reconnect WiFi after %d attempts", 
        ( WiFi.status() == WL_CONNECTED) ? "Successful" : "Unsuccessful", reconnectAttempt );

      addToEventLogfile( item );
      reconnectReport = reconnectAttempt;
    }
  } // reconnectAttempt>0

  if( restartTimer.lapsed() ) {
    // After three unsuccessful attempts, restart the ESP
    addToEventLogfile( "Restarting ESP due to loss of WiFi connection" );
    ESP.restart();
  };

}; // loopWifi()
