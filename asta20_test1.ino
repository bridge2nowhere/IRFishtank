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

/*Ch1 0xFFC03F
Ch1 + 0xFF30CF
Ch1 - 0xFF906F

Ch2 0xFFE01F
Ch2 + 0xFF50AF
Ch2 -  0xFFA857

Ch3. 0xFF20DF
Ch3 +  0xFFB04F
Ch3 -  0xFF6897

Ch4. 0xFF609F
Ch4 + 0xFF708F
Ch4 - 0xFF28D7
 */

const unsigned long irCh1 =  0xFFC03FUL;
const unsigned long irCh1P = 0xFF30CFUL;
const unsigned long irCh1M = 0xFF906FUL;

const unsigned long irCh2 =  0xFFE01FUL;
const unsigned long irCh2P = 0xFF50AFUL;
const unsigned long irCh2M = 0xFFA857UL;


const unsigned long irCh3 =  0xFF20DFUL;
const unsigned long irCh3P = 0xFFB04FUL;
const unsigned long irCh3M = 0xFF6897UL;


const unsigned long irCh4 =  0xFF609FUL;
const unsigned long irCh4P = 0xFF708FUL;
const unsigned long irCh4M = 0xFF28D7UL;

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



void loop()
{
//test code goes between the two bars
/////////////////////////////////////////////////////
  //set ch2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  
  //test + and -
  irsend.sendNEC(irCh2P,32);
  delay(sendDelay);
  irsend.sendNEC(irCh2P,32);
  delay(sendDelay);
  irsend.sendNEC(irCh2P,32);
  delay(sendDelay);
  irsend.sendNEC(irCh2P,32);
  delay(sendDelay);
  irsend.sendNEC(irCh2P,32);
  delay(sendDelay);
  irsend.sendNEC(irCh2M,32);
  delay(sendDelay);
  irsend.sendNEC(irCh2M,32);
  delay(sendDelay);
  irsend.sendNEC(irCh2M,32);
  delay(sendDelay);
  irsend.sendNEC(irCh2M,32);
  delay(sendDelay);
  irsend.sendNEC(irCh2M,32);
  delay(sendDelay);

////////////////////////////////////////////////////


  oled.clear();
  oled.println("Done");
//  oled.println("ch1 @ ");
//  oled.println(chan1);
//  
//  oled.print(" ch2 @ ");
//  oled.println(chan2);
//  
//  oled.print("ch3 @ ");
//  oled.println(chan3);
//  
//  oled.print("ch4 @ ");
//  oled.println(chan4);
  
}  
