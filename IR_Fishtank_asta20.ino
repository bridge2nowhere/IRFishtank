#include <Wire.h>
#include "RTClib.h"
#include <IRremote.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define TODBUTTON 4
#define OVERRIDEBUTTON 5

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

int sendDelay = 2000;




SSD1306AsciiWire oled;
IRsend irsend;
RTC_DS3231 rtc;

//sets the different options for time events
enum class TimeOfDay : uint8_t
{
  Q0,Q1,Q2,Q3,Q4,Q5,Q6,Q7,Q8,Q9,Q10,Q11
};

//initializes the time keeper variables
TimeOfDay tod = TimeOfDay::Q1;
TimeOfDay newTod = TimeOfDay::Q0;


byte chan1 = 0;
byte chan2 = 0;
byte chan3 = 0;
byte chan4 = 0;
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

  if (rtc.lostPower()) {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  
  //uncomment line below to force time update to the time and date the sketch was compiled then recomment and reupload
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
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
  oled.clear();
  oled.println("Running");
  if (overrideTod)
   {
      if (newTod == TimeOfDay::Q1  && tod != newTod) {
        tod = TimeOfDay::Q1;
        q1();
      }
      else if (newTod == TimeOfDay::Q2  && tod != newTod) {
        tod = TimeOfDay::Q2;
        q2();
      }
      else if (newTod == TimeOfDay::Q3  && tod != newTod){
        tod = TimeOfDay::Q3;
        q3();
      }
      else if (newTod == TimeOfDay::Q4  && tod != newTod){
        tod = TimeOfDay::Q4;
        q4();
      }
      else if (newTod == TimeOfDay::Q5  && tod != newTod){
        tod = TimeOfDay::Q5;
        q5();
      }
      else if (newTod == TimeOfDay::Q6  && tod != newTod){
        tod = TimeOfDay::Q6;
        q6();
      }
      else if (newTod == TimeOfDay::Q7  && tod != newTod){
        tod = TimeOfDay::Q7;
        q7();
      }
      else if (newTod == TimeOfDay::Q8  && tod != newTod){
        tod = TimeOfDay::Q8;
        q8();
      }
      else if (newTod == TimeOfDay::Q9 && tod != newTod){
        tod = TimeOfDay::Q9;
        q9();
      }
      else if (newTod == TimeOfDay::Q10  && tod != newTod){
        tod = TimeOfDay::Q10;
        q10();
      }
      else if (newTod == TimeOfDay::Q11){
        tod = TimeOfDay::Q11;
        q11();
      }
   }

  
  else
  {  
    forceTod = false;
    if(now.hour() == 7 && now.minute () < 30  && tod !=TimeOfDay::Q1){
        newTod = TimeOfDay::Q1;
        tod = TimeOfDay::Q1;
        q1();
      }
    else if((now.hour() == 7 && now.minute() >= 30) || (now.hour() == 8  && now.minute() < 30)  && tod !=TimeOfDay::Q1){
        newTod = TimeOfDay::Q2;
        tod = TimeOfDay::Q2;
        q2();
      }
    else if((now.hour() == 8  && now.minute() < 30) || (now.hour() >= 9 && now.hour() < 12)  && tod !=TimeOfDay::Q3){
        newTod = TimeOfDay::Q3;
        tod = TimeOfDay::Q3;
        q3();
      }
    else if(now.hour() >= 12 && now.hour() < 14  && tod !=TimeOfDay::Q4){
        newTod = TimeOfDay::Q4;
        tod = TimeOfDay::Q4;
        q4();
      }
    else if(now.hour() >= 14 && now.hour() < 17  && tod !=TimeOfDay::Q5){
        newTod = TimeOfDay::Q5;
        tod = TimeOfDay::Q5;
       q5();
      }
    else if(now.hour() >= 17 && now.hour() < 21  && tod !=TimeOfDay::Q6){
        newTod = TimeOfDay::Q6;
        tod = TimeOfDay::Q6;
       q6();
      }
    else if(now.hour() >= 21 && now.hour() < 22  && tod !=TimeOfDay::Q7){
        newTod = TimeOfDay::Q7;
        tod = TimeOfDay::Q7;
       q7();
      }
     else if(now.hour() == 22 && now.minute() < 30  && tod !=TimeOfDay::Q8){
        newTod = TimeOfDay::Q8;
        tod = TimeOfDay::Q8;
       q8();
      }
    else if(now.hour() == 22 && now.minute() >= 30  && tod !=TimeOfDay::Q9){
        newTod = TimeOfDay::Q9;
        tod = TimeOfDay::Q9;
       q9();
      }
    else if(now.hour() >= 23  && tod !=TimeOfDay::Q10){
        newTod = TimeOfDay::Q10;
        tod = TimeOfDay::Q10;
       q10();
      }
    else if(now.hour() >= 0 && now.hour() < 7  && tod !=TimeOfDay::Q11){
        newTod = TimeOfDay::Q11;
        tod = TimeOfDay::Q11;
       q11();
      }                  
  }
}




void updateDisplay(DateTime now)
{
  //prints current time
  oled.set1X();
  oled.setFont(ZevvPeep8x16);
  oled.clear();
  oled.print(now.hour(), DEC);
  oled.print(':');
  if(now.minute() < 10) oled.print('0');
  oled.print(now.minute(), DEC);
  
  //prints Override status
  if(overrideTod == true) oled.println("   Override");
  else oled.println();
  
  //prints tod
  oled.set2X();
  //oled.setFont(Arial_bold_14);
  if(tod == TimeOfDay::Q1) oled.println("Q1");
  else if(tod == TimeOfDay::Q2) oled.println("Q2");
  else if(tod == TimeOfDay::Q3) oled.println("Q3");
  else if(tod == TimeOfDay::Q4) oled.println("Q4");
  else if(tod == TimeOfDay::Q5) oled.println("Q5");
  else if(tod == TimeOfDay::Q6) oled.println("Q6");
  else if(tod == TimeOfDay::Q7) oled.println("Q7");
  else if(tod == TimeOfDay::Q8) oled.println("Q8");
  else if(tod == TimeOfDay::Q9) oled.println("Q9");
  else if(tod == TimeOfDay::Q10) oled.println("Q10");
  else if(tod == TimeOfDay::Q11) oled.println("Q11");
  oled.set1X();
  oled.setFont(font5x7);
  oled.print("ch1 @ ");
  oled.print(chan1);
  if(chan1 < 10) oled.print(" ");
  oled.print("  ch2 @ ");
  oled.println(chan2);
  oled.print("ch3 @ ");
  oled.print(chan3);
  if(chan3 < 10) oled.print(" ");
  oled.print("  ch4 @ ");
  oled.println(chan4);
}

TimeOfDay nextEnum()
{
  if(tod == TimeOfDay::Q1) return TimeOfDay::Q2;
  else if(tod == TimeOfDay::Q2) return TimeOfDay::Q3;
  else if(tod == TimeOfDay::Q3) return TimeOfDay::Q4;
  else if(tod == TimeOfDay::Q4) return TimeOfDay::Q5;
  else if(tod == TimeOfDay::Q5) return TimeOfDay::Q6;
  else if(tod == TimeOfDay::Q6) return TimeOfDay::Q7;
  else if(tod == TimeOfDay::Q7) return TimeOfDay::Q8;
  else if(tod == TimeOfDay::Q8) return TimeOfDay::Q9;
  else if(tod == TimeOfDay::Q9) return TimeOfDay::Q10;
  else if(tod == TimeOfDay::Q10) return TimeOfDay::Q11;
  else if(tod == TimeOfDay::Q11) return TimeOfDay::Q1;
}


void q1()
{
  //chan1
  irsend.sendNEC(irCh1,32);
  delay(sendDelay);
  if(chan1 < 6)
  {
    while (chan1 < 6) {
      chan1 ++;
      irsend.sendNEC(irCh1P,32);
      delay(sendDelay);
    }
  }
  else if(chan1 > 6) {
    while (chan1 > 6) {
      chan1 --;
      irsend.sendNEC(irCh1M,32);
      delay(sendDelay);
    }    
  }
  
  //chan2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(irCh2M,32);
    delay(sendDelay);
  }

  //chan3
  irsend.sendNEC(irCh3,32);
  delay(sendDelay);
  while (chan3 > 0) {
    chan3 --;
    irsend.sendNEC(irCh3M,32);
    delay(sendDelay);
  }

  //chan4
  irsend.sendNEC(irCh4,32);
  delay(sendDelay);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(irCh4M,32);
    delay(sendDelay);
  }

}

void q2()
{
  //chan1
  irsend.sendNEC(irCh1,32);
  delay(sendDelay);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(irCh1P,32);
    delay(sendDelay);
  }
  
  //chan2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(irCh2M,32);
    delay(sendDelay);
  }

  //chan3
  irsend.sendNEC(irCh3,32);
  delay(sendDelay);
  while (chan3 > 0) {
    chan3 --;
    irsend.sendNEC(irCh3M,32);
    delay(sendDelay);
  }

  //chan4
  irsend.sendNEC(irCh4,32);
  delay(sendDelay);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(irCh4M,32);
    delay(sendDelay);
  }

}


void q3()
{
  //chan1
  irsend.sendNEC(irCh1,32);
  delay(sendDelay);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(irCh1P,32);
    delay(sendDelay);
  }
  
  //chan2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(irCh2M,32);
    delay(sendDelay);
  }

  //chan3
  irsend.sendNEC(irCh3,32);
  delay(sendDelay);
  while (chan3 < 11) {
    chan3 ++;
    irsend.sendNEC(irCh3P,32);
    delay(sendDelay);
  }

  //chan4
  irsend.sendNEC(irCh4,32);
  delay(sendDelay);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(irCh4M,32);
    delay(sendDelay);
  }
}

void q4()
{
  //chan1
  irsend.sendNEC(irCh1,32);
  delay(sendDelay);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(irCh1P,32);
    delay(sendDelay);
  }
  
  //chan2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(irCh2M,32);
    delay(sendDelay);
  }

  //chan3
  irsend.sendNEC(irCh3,32);
  delay(sendDelay);
  while (chan3 < 11) {
    chan3 ++;
    irsend.sendNEC(irCh3P,32);
    delay(sendDelay);
  }

  //chan4
  irsend.sendNEC(irCh4,32);
  delay(sendDelay);
  while (chan4 < 11) {
    chan4 ++;
    irsend.sendNEC(irCh4P,32);
    delay(sendDelay);
  }
  
}

void q5()
{
  //chan1
  irsend.sendNEC(irCh1,32);
  delay(sendDelay);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(irCh1P,32);
    delay(sendDelay);
  }
  
  //chan2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  if(chan2 < 5)
  {
    while(chan2 < 5)
    {
      chan2 ++;
      irsend.sendNEC(irCh2P,32);
      delay(sendDelay);
    }
  }
  else if(chan2 > 5)
  {
    while (chan2 > 5) {
      chan2 --;
      irsend.sendNEC(irCh2M,32);
      delay(sendDelay);
    }
  }

  //chan3
  irsend.sendNEC(irCh3,32);
  delay(sendDelay);
  while (chan3 < 11) {
    chan3 ++;
    irsend.sendNEC(irCh3P,32);
    delay(sendDelay);
  }

  //chan4
  irsend.sendNEC(irCh4,32);
  delay(sendDelay);
  while (chan4 < 11) {
    chan4 ++;
    irsend.sendNEC(irCh4P,32);
    delay(sendDelay);
  }  
}

void q6()
{
  //chan1
  irsend.sendNEC(irCh1,32);
  delay(sendDelay);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(irCh1P,32);
    delay(sendDelay);
  }
  
  //chan2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  while(chan2 < 11)
  {
    chan2 ++;
    irsend.sendNEC(irCh2P,32);
    delay(sendDelay);
  }
  
  //chan3
  irsend.sendNEC(irCh3,32);
  delay(sendDelay);
  while (chan3 < 11) {
    chan3 ++;
    irsend.sendNEC(irCh3P,32);
    delay(sendDelay);
  }

  //chan4
  irsend.sendNEC(irCh4,32);
  delay(sendDelay);
  while (chan4 < 11) {
    chan4 ++;
    irsend.sendNEC(irCh4P,32);
    delay(sendDelay);
  }  
}

void q7()
{
  //chan1
  irsend.sendNEC(irCh1,32);
  delay(sendDelay);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(irCh1P,32);
    delay(sendDelay);
  }
  
  //chan2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  while (chan2 > 0) 
  {
    chan2 --;
    irsend.sendNEC(irCh2M,32);
    delay(sendDelay);
  }
  
  //chan3
  irsend.sendNEC(irCh3,32);
  delay(sendDelay);
  while (chan3 < 11) {
    chan3 ++;
    irsend.sendNEC(irCh3P,32);
    delay(sendDelay);
  }

  //chan4
  irsend.sendNEC(irCh4,32);
  delay(sendDelay);
  while (chan4 < 11) {
    chan4 ++;
    irsend.sendNEC(irCh4P,32);
    delay(sendDelay);
  }  
  
}

void q8()
{
  //chan1
  irsend.sendNEC(irCh1,32);
  delay(sendDelay);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(irCh1P,32);
    delay(sendDelay);
  }
  
  //chan2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  while (chan2 > 0) 
  {
    chan2 --;
    irsend.sendNEC(irCh2M,32);
    delay(sendDelay);
  }
  
  //chan3
  irsend.sendNEC(irCh3,32);
  delay(sendDelay);
  while (chan3 < 11) {
    chan3 ++;
    irsend.sendNEC(irCh3P,32);
    delay(sendDelay);
  }

  //chan4
  irsend.sendNEC(irCh4,32);
  delay(sendDelay);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(irCh4M,32);
    delay(sendDelay);
  }

}

void q9()
{
  //chan1
  irsend.sendNEC(irCh1,32);
  delay(sendDelay);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(irCh1P,32);
    delay(sendDelay);
  }
  
  //chan2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  while (chan2 > 0) 
  {
    chan2 --;
    irsend.sendNEC(irCh2M,32);
    delay(sendDelay);
  }
  
  //chan3
  irsend.sendNEC(irCh3,32);
  delay(sendDelay);
  while (chan3 > 0) {
    chan3 --;
    irsend.sendNEC(irCh3M,32);
    delay(sendDelay);
  }

  //chan4
  irsend.sendNEC(irCh4,32);
  delay(sendDelay);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(irCh4M,32);
    delay(sendDelay);
  }
}

void q10()
{
  //chan1
  irsend.sendNEC(irCh1,32);
  delay(sendDelay);
  if(chan1 < 2)
  {
    while (chan1 < 2) {
      chan1 ++;
      irsend.sendNEC(irCh1P,32);
      delay(sendDelay);
    }
  }
  else if(chan1 > 2) {
    while (chan1 > 2) {
      chan1 --;
      irsend.sendNEC(irCh1M,32);
      delay(sendDelay);
    }    
  }
  
  //chan2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  while (chan2 > 0) 
  {
    chan2 --;
    irsend.sendNEC(irCh2M,32);
    delay(sendDelay);
  }
  
  //chan3
  irsend.sendNEC(irCh3,32);
  delay(sendDelay);
  while (chan3 > 0) {
    chan3 --;
    irsend.sendNEC(irCh3M,32);
    delay(sendDelay);
  }

  //chan4
  irsend.sendNEC(irCh4,32);
  delay(sendDelay);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(irCh4M,32);
    delay(sendDelay);
  }

}

void q11()
{
  //chan1
  irsend.sendNEC(irCh1,32);
  delay(sendDelay);
  while (chan1 > 0) 
  {
    chan1 --;
    irsend.sendNEC(irCh1M,32);
    delay(sendDelay);
  }
  
  //chan2
  irsend.sendNEC(irCh2,32);
  delay(sendDelay);
  while (chan2 > 0) 
  {
    chan2 --;
    irsend.sendNEC(irCh2M,32);
    delay(sendDelay);
  }
  
  //chan3
  irsend.sendNEC(irCh3,32);
  delay(sendDelay);
  while (chan3 > 0) {
    chan3 --;
    irsend.sendNEC(irCh3M,32);
    delay(sendDelay);
  }

  //chan4
  irsend.sendNEC(irCh4,32);
  delay(sendDelay);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(irCh4M,32);
    delay(sendDelay);
  }
}
