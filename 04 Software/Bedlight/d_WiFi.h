#pragma once

#include <WiFi.h>
#include <WiFiMulti.h>
#include <map>

#include "a_Data.h"

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

  // Check if WiFi is still alive
  if ( WiFi.status() == WL_CONNECTED) {  /*if the connection lost it will connect to next network*/
    if( !wifiConnectReported ) {
      Serial.printf( "WiFi connected to SSID %s signal strength %ddB\n", WiFi.SSID().c_str(), WiFi.RSSI() );
      wifiConnectReported=true;
      wifiNotConnectReported=false;
      reconnectTimer1.reset();
      reconnectTimer2.reset();
      reconnectTimer3.reset();
      restartTimer.reset();
    };
  }
  else {
    if( !wifiNotConnectReported ) {
      Serial.println("WiFi not connected!");  /*if all conditions fail print this*/
      wifiNotConnectReported=true;
      wifiConnectReported=false;
    }
  }

  if( reconnectTimer1.lapsed() || reconnectTimer2.lapsed() || reconnectTimer3.lapsed() ) {
    // Try to connect to the strongest available network
    wifiMulti.run( CONNECT_TIMEOUT_MS );
  };

  if( restartTimer.lapsed() ) {
    // After three unsuccessful attempts, restart the ESP
    ESP.restart();
  };

};
