#pragma once

#include <WiFi.h>
#include <WiFiMulti.h>
#include <map>

#include "Data.h"

const uint32_t CONNECT_TIMEOUT_MS = 10000;

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
  if (wifiMulti.run( CONNECT_TIMEOUT_MS ) == WL_CONNECTED) {  /*if the connection lost it will connect to next network*/
    if( !wifiConnectReported ) {
      Serial.printf( "WiFi connected to SSID %s signal strength %ddB\n", WiFi.SSID().c_str(), WiFi.RSSI() );
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
};
