#include <Wire.h>
#include "RTClib.h"
#include <IRremote.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define TODBUTTON 4
#define OVERRIDEBUTTON 5


SSD1306AsciiWire oled;
IRsend irsend;
RTC_DS3231 rtc;

//sets the different options for time events
enum class TimeOfDay : uint8_t
{
  Night, Dawn, Day, Dusk, Evening, Storm
};

//initializes the time keeper variables
TimeOfDay tod = TimeOfDay::Night;
TimeOfDay newTod = TimeOfDay :: Storm;

boolean powerWhite = true;
boolean powerRGB = false;
boolean todSent = false;
boolean overrideTod = false;
boolean forceTod = false;

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  oled.begin(&Adafruit128x64, 0x3C);

  pinMode(TODBUTTON,INPUT_PULLUP);
  pinMode(OVERRIDEBUTTON,INPUT_PULLUP);
  
  //updates the time and date to the time and date the sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  //turns override mode on and off
  if(digitalRead(OVERRIDEBUTTON) == LOW)
  {
    overrideTod = !overrideTod;
  }
  //force next tod;
  if (digitalRead(TODBUTTON) == LOW)
  {
    newTod = nextEnum();
  }

  DateTime now = rtc.now();
  scheduler(now);
  updateDisplay(now);
  delay(3000);
}


void scheduler(DateTime now)
{
  if (overrideTod)
   {
      if (newTod == TimeOfDay::Day && tod != newTod) {
        tod = TimeOfDay::Day;
        Day();
      }
      else if (newTod == TimeOfDay::Dawn && tod != newTod) {
        tod = TimeOfDay::Dawn;
        dawn();
      }
      else if (newTod == TimeOfDay::Dusk && tod != newTod){
        tod = TimeOfDay::Dusk;
        dusk();
      }
      else if (newTod == TimeOfDay::Evening && tod != newTod){
        tod = TimeOfDay::Evening;
        evening();
      }
      else if (newTod == TimeOfDay::Night && tod != newTod){
        tod = TimeOfDay::Night;
        night();
      }
   }
  else
  {  
    forceTod = false;
    if(now.hour() >= 6 && now.hour() < 7 && tod !=TimeOfDay::Dawn){
        newTod = TimeOfDay::Dawn;
        tod = TimeOfDay::Dawn;
        dawn();
      }
    else if(now.hour() >= 7 && now.hour() < 18 && tod !=TimeOfDay::Day){
        newTod = TimeOfDay::Day;
        tod = TimeOfDay::Day;
        Day();
      }
    else if(now.hour() >= 18 && now.hour() < 19 && tod !=TimeOfDay::Dusk){
        newTod = TimeOfDay::Dusk;
        tod = TimeOfDay::Dusk;
        dusk();
      }
    else if(now.hour() >= 19 && now.hour() < 22 && tod !=TimeOfDay::Evening){
        newTod = TimeOfDay::Evening;
        tod = TimeOfDay::Evening;
        evening();
      }
    else if(now.hour() >= 22 || now.hour() < 6 && tod !=TimeOfDay::Night){
        newTod = TimeOfDay::Night;
        tod = TimeOfDay::Night;
       night();
      }
  }
}




void updateDisplay(DateTime now)
{
  
  oled.set1X();
  oled.setFont(ZevvPeep8x16);
  oled.clear();

  //prints current time
  oled.print(now.hour(), DEC);
  oled.print(':');
  if(now.minute() < 10) oled.print('0');
  oled.println(now.minute(), DEC);
  
  //prints Override status
  if(overrideTod == true) oled.println("Override");
  else oled.println();
  
  //prints tod
  oled.set2X();
  if(tod == TimeOfDay::Night) oled.println("Night");
  else if(tod == TimeOfDay::Dawn) oled.println("Dawn");
  else if(tod == TimeOfDay::Day) oled.println("Day");
  else if(tod == TimeOfDay::Dusk) oled.println("Dusk");
  else if(tod == TimeOfDay::Evening) oled.println("Evening");
}

TimeOfDay nextEnum()
{
  if(tod == TimeOfDay::Night) return TimeOfDay::Dawn;
  else if(tod == TimeOfDay::Dawn) return TimeOfDay::Day;
  else if(tod == TimeOfDay::Day) return TimeOfDay::Dusk;
  else if(tod == TimeOfDay::Dusk) return TimeOfDay::Evening;
  else if(tod == TimeOfDay::Evening) return TimeOfDay::Night;
  else return TimeOfDay::Day;
}




/////////////////////////////
// Events
//whPower = 0xF7C03F;
//whPlus = 0xF7E01F;
//whMinus = 0xF7D02F;
//storm = 0xF7C837;
//rgbPower = 0xF740BF;
//blue = 0xF750AF;
//amber = 0xF708F7;
//pink = 0xF76897;
////////////////////////////////////


void night() 
{
  if (powerWhite) {
    irsend.sendNEC(0xF7C03F,32);
    powerWhite = !powerWhite;
  }
  if (powerRGB) 
  {
    irsend.sendNEC(0xF740BF,32);
    powerRGB = !powerRGB;
  }
}

void dawn()
{
  if (powerWhite) {
    irsend.sendNEC(0xF7C03F,32);
    powerWhite = !powerWhite;
  }
  if (!powerRGB) 
  {
    irsend.sendNEC(0xF740BF,32);
     powerRGB = !powerRGB;
  }
  irsend.sendNEC(0xF708F7,32);
}

void Day()
{
  if (!powerWhite) {
    irsend.sendNEC(0xF7C03F,32);
    powerWhite = !powerWhite;
  }
  if (powerRGB) 
  {
    irsend.sendNEC(0xF740BF,32);
     powerRGB = !powerRGB;
  }
}

void dusk()
{
  if (powerWhite) {
    irsend.sendNEC(0xF7C03F,32);
    powerWhite = !powerWhite;
  }
  if (!powerRGB) 
  {
    irsend.sendNEC(0xF740BF,32);
     powerRGB = !powerRGB;
  }
  irsend.sendNEC(0xF76897,32);
}

void evening()
{
  if (powerWhite) {
    irsend.sendNEC(0xF7C03F,32);
    powerWhite = !powerWhite;
  }
  if (!powerRGB) 
  {
    irsend.sendNEC(0xF740BF,32);
     powerRGB = !powerRGB;
  }
  irsend.sendNEC(0xF750AF,32);
}
