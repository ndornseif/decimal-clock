/*
  DecimalClock.ino
  LDM8E decimal clock for LDM2 module display.
  This program displays the current time on a LDM8E as decimal time.
  Created 2023-10-22 by N.Dornseif
  Writen for the LDM8E hardware only.
  More info on the LDM8E-Library:
  https://github.com/ndornseif/LDM8E-Library
  More info on the LDM8E hardware:
  https://github.com/ndornseif/LDM8E-LEDDisplayController
  Published under GPL-3.0 license.
*/
#include <LDM8EDriver.h>
#include <WiFi.h>
#include "time.h"
#include "sntp.h"

/*
  This parameter defines the dimming mode
  0=Chip Enable PWM (LDM2)
  1=Split ground PWM (LDM1)
  2=Analog dimming (Both)
  See github for more info on dimming:
  https://github.com/ndornseif/LDM8E-LEDDisplayController
*/
const byte dimmingMode = 2;

//Defines the number of diplay modules connected
//Having fewer than four reduces your display precision
const byte numModules = 4;

//Enter Wifi credentials here
const char* ssid       = "";
const char* password   = "";

//Consider replacing with ntp servers in your region
const char* ntpServer1 = "ptbtime1.ptb.de";
const char* ntpServer2 = "ptbtime4.ptb.de";

//Configure your timezone here
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
const char* time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";

//Configure the brightness during day and night
//255 = max, 0 = min
const byte dayBrightness = 240;
const byte nightBrightness = 42;

//If the activity LED should blink when the time is updated
const bool blinkActyLED = true;

/*
Configure the time when night brightness is active
Set value as decimal time:
One increment corresponds to 8.64 seconds.
0000=00:00:00 ; 0001=00:00:08 ; 9999=23:59:51 ; 5000 = 12:00:00 
*/
const int nightStart = 8750; //21:00
const int nightEnds = 2083; //05:00

//If the decimal points should be used. 
//Will show "1.23.4" instead of "1234" 
const bool useDP = true;

//Set up a display named "mydisplay"
LDM8EDisplay mydisplay(dimmingMode, numModules);

void checkBrightness(int currentTime){
  if(currentTime > nightStart || currentTime < nightEnds){
    mydisplay.setBrightness(nightBrightness);
  }else{
    mydisplay.setBrightness(dayBrightness);
  }
}

int timeToDecimal(struct tm *t){
  word siminutes;
  word sihours;
  word siseconds;
  float totalsiseconds;
  char timeHour[3];
  char timeMinute[3];
  char timeSecond[3];
  strftime(timeHour,3, "%H", t);
  strftime(timeMinute,3, "%M", t);
  strftime(timeSecond,3, "%S", t);
  siminutes = ((timeMinute[0]-'0')*10)+(timeMinute[1]-'0');
  sihours = ((timeHour[0]-'0')*10)+(timeHour[1]-'0');
  siseconds = ((timeSecond[0]-'0')*10)+(timeSecond[1]-'0');
  totalsiseconds = (siminutes * 60) + (sihours * 3600) + siseconds;
  return (int)((totalsiseconds/86400.0) * 10000.0);
}

void setup() {
  //Initalize the display
  mydisplay.begin();
  //Turn off all segments
  mydisplay.setDisplay(0);
  //Set brightness to 50%
  mydisplay.setBrightness(128);

  //Turn on all "G" segments
  //Displays a line: "----"
  mydisplay.setDisplay(0x02020202);
  
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
  Serial.printf("Connecting to WiFi Network: %s ", ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
      delay(1000);
      Serial.print("-");
  }
  Serial.println("WiFi Connected");
}

void sendTimeToDisplay(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
  }else{
    int currentTime = timeToDecimal(&timeinfo);
    checkBrightness(currentTime);
    mydisplay.writeDisplay(currentTime, 0x01000100);
    Serial.println(currentTime);
  }
}

void loop() {
  delay(2000);
  mydisplay.setActyLED(blinkActyLED);
  sendTimeToDisplay();
  delay(50);
  mydisplay.setActyLED(0);
}
