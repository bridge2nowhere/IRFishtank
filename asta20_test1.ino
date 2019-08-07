#include <Wire.h>
#include "RTClib.h"
#include <IRremote.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

SSD1306AsciiWire oled;
IRsend irsend;
RTC_DS3231 rtc;


#define TODBUTTON 4
#define OVERRIDEBUTTON 5

const unsigned long irCh1 = 0xFFC03FUL;
const unsigned long irCh2 = 0xFFE01FUL;
const unsigned long irCh3 = 0xFF20DFUL;
const unsigned long irCh4 = 0xFF609FUL;
const unsigned long irPlus = 0xFF708FUL;
const unsigned long irMinus = 0xFF906FUL;

int sendDelay = 1000;

byte chan1 = 0;
byte chan2 = 0;
byte chan3 = 0;
byte chan4 = 0;

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  oled.begin(&Adafruit128x64, 0x3C);
  
  pinMode(TODBUTTON,INPUT_PULLUP);
  pinMode(OVERRIDEBUTTON,INPUT_PULLUP); 

  oled.setFont(ZevvPeep8x16); 
}



void looop()
{
//test code goes between the two bars
/////////////////////////////////////////////////////
  //set ch2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  
  //test + and -
  irsend.sendNEC(irPlus,32);
  delay(sendDelay);
  irsend.sendNEC(irPlus,32);
  delay(sendDelay);
  irsend.sendNEC(irPlus,32);
  delay(sendDelay);
  irsend.sendNEC(irPlus,32);
  delay(sendDelay);
  irsend.sendNEC(irPlus,32);
  delay(sendDelay);
  irsend.sendNEC(irMinus,32);
  delay(sendDelay);
  irsend.sendNEC(irMinus,32);
  delay(sendDelay);
  irsend.sendNEC(irMinus,32);
  delay(sendDelay);
  irsend.sendNEC(irMinus,32);
  delay(sendDelay);
  irsend.sendNEC(irMinus,32);
  delay(sendDelay);

////////////////////////////////////////////////////


  oled.clear();
  oled.println("Done")
  oled.println("ch1 @ ");
  oled.println(chan1);
  
  oled.print(" ch2 @ ");
  oled.println(chan2);
  
  oled.print("ch3 @ ");
  oled.println(chan3);
  
  oled.print("ch4 @ ");
  oled.println(chan4);
  
}  
  
  
  
