#include <Arduino.h>
#include <WiFi.h>           
#include <HTTPClient.h>
#include <NTPClient.h>     
#include "ArduinoJson.h"
#include "time.h"

/*
https://weerlive.nl/delen.php

De API lees je als volgt:
plaats	de locatie
temp	actuele temperatuur
gtemp	gevoelstemperatuur
samenv	omschrijving weersgesteldheid
lv	relatieve luchtvochtigheid
windr	windrichting
windms	windsnelheid in meter per seconde
winds	windkracht (Beaufort)
windk	Windsnelheid in knopen
windkmh	Windsnelheid in km/h
luchtd	luchtdruk
ldmmhg	luchtdruk in mm-kwikdruk
dauwp	dauwpunt
zicht	zicht in km
verw	korte dagverwachting
sup	zon op
sunder	zon onder
image	afbeeldingsnaam
d0weer	Weericoon vandaag
d0tmax	Maxtemp vandaag
d0tmin	Mintemp vandaag
d0windk	Windkracht vandaag (in Bft)
d0windr	Windrichting vandaag
d0neerslag  	Neerslagkans vandaag (%)
d0zon	Zonkans vandaag (%)
d1weer	Weericoon morgen
d1tmax	Maxtemp morgen
d1tmin	Mintemp morgen
d1windk	Windkracht morgen (in Bft)
d1windr	Windrichting morgen
d1neerslag	Neerslagkans morgen (%)
d1zon	Zonkans morgen (%)
d2tmax	Maxtemp overmorgen
d2tmin	Mintemp overmorgen
d2weer	Weericoon overmorgen
d2windk	Windkracht overmorgen (in Bft)
d2windr	Windrichting overmorgen
d2neerslag	Neerslagkans overmorgen (%)
d2zon	Zonkans overmorgen (%)
alarm	Geldt er een weerwaarschuwing voor deze regio of niet?
alarmtxt	Als er sprake is van een waarschuwing, verschijnt alarmtxt in de API. Hier lees je de omschrijving van de weersituatie.
*/ 


typedef struct {
  const char* weer;
  float tmax;
  float tmin;
  float windk;
  float windknp;
  float windms;
  float windkmh;
  const char* windr;
  float neerslag;
  float zon;
} otherDayWeather_t;
      
class WeatherAPI {
  public:

    WeatherAPI() { request(); };

    const char* plaats;
    float temp;
    float gtemp;
    const char* samenv;
    float lv;
    const char* windr;
    float windms;
    float winds;
    float windk;
    float windkmh;
    float luchtd;
    float ldmmhg;
    float dauwp;
    float zicht;
    const char* verw;
    const char* sup;
    const char* sunder;
    
    const char* image;
    const char* alarm;

    otherDayWeather_t d0, d1, d2;

    void request() {
      const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(49) + 1200;
      DynamicJsonDocument doc(capacity);

      HTTPClient http;                                // Initialise HTTP Client
      http.begin("http://weerlive.nl/api/json-data-10min.php?key=demo&locatie=Amsterdam");    

      http.GET();                                  //Send the request
      String json = http.getString();                            // Return to the string
      http.end();   //Close connection                            // Close http client
      
      deserializeJson(doc, json);

      JsonObject liveweer = doc["liveweer"][0];

      plaats = liveweer["plaats"]; // "Amsterdam"
      temp = liveweer["temp"]; // "12.1"
      gtemp = liveweer["gtemp"]; // "6.6"
      samenv = liveweer["samenv"]; // "Zwaar bewolkt"
      lv = liveweer["lv"]; // "79"
      windr = liveweer["windr"]; // "Zuid"
      windms = liveweer["windms"]; // "8"
      winds = liveweer["winds"]; // "5"
      windk = liveweer["windk"]; // "15.6"
      windkmh = liveweer["windkmh"]; // "28.8"
      luchtd = liveweer["luchtd"]; // "1008.0"
      ldmmhg = liveweer["ldmmhg"]; // "756"
      dauwp = liveweer["dauwp"]; // "8"
      zicht = liveweer["zicht"]; // "19"
      verw = liveweer["verw"]; // "Zacht en wisselvallig"
      sup = liveweer["sup"]; // "07:36"
      sunder = liveweer["sunder"]; // "17:12"
      
      image = liveweer["image"]; // "bewolkt"
      
      d0.weer     = liveweer["d0weer"]; // "bewolkt"
      d0.tmax     = liveweer["d0tmax"]; // "17"
      d0.tmin     = liveweer["d0tmin"]; // "9"
      d0.windk    = liveweer["d0windk"]; // "4"
      d0.windknp  = liveweer["d0windknp"]; // "16"
      d0.windms   = liveweer["d0windms"]; // "8"
      d0.windkmh  = liveweer["d0windkmh"]; // "30"
      d0.windr    = liveweer["d0windr"]; // "Z"
      d0.neerslag = liveweer["d0neerslag"]; // "4"
      d0.zon      = liveweer["d0zon"]; // "4"

      d1.weer     = liveweer["d1weer"]; // "bewolkt"
      d1.tmax     = liveweer["d1tmax"]; // "17"
      d1.tmin     = liveweer["d1tmin"]; // "9"
      d1.windk    = liveweer["d1windk"]; // "4"
      d1.windknp  = liveweer["d1windknp"]; // "16"
      d1.windms   = liveweer["d1windms"]; // "8"
      d1.windkmh  = liveweer["d1windkmh"]; // "30"
      d1.windr    = liveweer["d1windr"]; // "Z"
      d1.neerslag = liveweer["d1neerslag"]; // "4"
      d1.zon      = liveweer["d1zon"]; // "4"

      d2.weer     = liveweer["d2weer"]; // "bewolkt"
      d2.tmax     = liveweer["d2tmax"]; // "17"
      d2.tmin     = liveweer["d2tmin"]; // "9"
      d2.windk    = liveweer["d2windk"]; // "4"
      d2.windknp  = liveweer["d2windknp"]; // "16"
      d2.windms   = liveweer["d2windms"]; // "8"
      d2.windkmh  = liveweer["d2windkmh"]; // "30"
      d2.windr    = liveweer["d2windr"]; // "Z"
      d2.neerslag = liveweer["d2neerslag"]; // "4"
      d2.zon      = liveweer["d2zon"]; // "4"
        
      alarm = liveweer["alarm"]; // "0"
    }
};

const char* ssid = "********";
const char* password = "********";

WiFiUDP ntpUDP;                                 //initialise UDP NTP
NTPClient ntpClient(ntpUDP, "time.google.com"); // initialist NTP client with server name 
HTTPClient http;                                // Initialise HTTP Client

/*
Code for clock can be found here:
https://github.com/myk3y/AutoClock/blob/master/Autoclock.ino
*/

void setup() {
  // Delays to try prevent brownout on power up
  delay(2000);

  Serial.begin(115200);

  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(49) + 1200;
  Serial.printf("Capacity: %d\n",capacity);
  Serial.printf("Free memory: %d\n",ESP.getFreeHeap());

  // Overwrite capacity for debugging
  DynamicJsonDocument doc(30000);

  Serial.printf("Free memory: %d\n",ESP.getFreeHeap());

  delay(2000);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  WeatherAPI weather;

  http.begin("http://worldtimeapi.org/api/timezone/Europe/Amsterdam.txt");    
  Serial.println("Still alive after http.begin");

  http.GET();                         // Send the request
  Serial.println("Still alive after http GET");
  
  String json = http.getString();     // Return to the string
  Serial.println("Still alive after get JSON string");

  http.end();   //Close connection    // Close http client
  Serial.println("Still alive after http end");

  WiFi.disconnect(true);
  Serial.println("Still alive after wifi disconnect");

  //Serial.println(json);

  Serial.printf("Free memory: %d\n",ESP.getFreeHeap());
  
  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }


  Serial.printf("Free memory: %d\n",ESP.getFreeHeap());

  Serial.println("Still alive before the optional part begins");

  //const char* abbreviation = doc["abbreviation"]; // "CET"
  //const char* client_ip = doc["client_ip"]; // "82.161.181.173"
  const char* datetime = doc["datetime"]; // "2020-11-01T18:42:25.255175+01:00"
  int day_of_week = doc["day_of_week"]; // 0
  int day_of_year = doc["day_of_year"]; // 306
  bool dst = doc["dst"]; // false
  int dst_offset = doc["dst_offset"]; // 0
  int raw_offset = doc["raw_offset"]; // 3600
  //const char* _timezone = doc["timezone"]; // "Europe/Amsterdam"
  long unixtime = doc["unixtime"]; // 1604252545
  //const char* utc_datetime = doc["utc_datetime"]; // "2020-11-01T17:42:25.255175+00:00"
  //const char* utc_offset = doc["utc_offset"]; // "+01:00"
  int week_number = doc["week_number"]; // 44

  Serial.printf("Day of week: %d\n", day_of_week);
  //Serial.printf("Datetime: %s\n", doc["datetime"]);

  /*
  struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
  }; 
  */

  /*
  timezone tz = {-60, 0};
  tm settime;
  settime.tm_hour=12;
  settime.tm_min=34;
  settime.tm_sec=56;
  settimeofday(settime,&tz);
  */

  time_t now;
  char strftime_buf[64];
  struct tm timeinfo;

  time(&now);
  // Set timezone to China Standard Time
  // setenv("TZ", "CST-8", 1);
  // tzset();

  //localtime_r(&now, &timeinfo);
  //strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
  //ESP_LOGI(TAG, "The current date/time in Shanghai is: %s", strftime_buf);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.printf("Free memory: %d\n",ESP.getFreeHeap());
  delay(5000);
}