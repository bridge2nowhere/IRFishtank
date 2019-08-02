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
  irsend.sendNEC(0xFFC03F,32);
  if(chan1 < 6)
  {
    while (chan1 < 6) {
      chan1 ++;
      irsend.sendNEC(0xFF708F,32);
    }
  }
  else if(chan1 > 6) {
    while (chan1 > 6) {
      chan1 --;
      irsend.sendNEC(0xFF708F,32);
    }    
  }
  
  //chan2
  irsend.sendNEC(0xFFE01F,32);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(0xFF906F,32);
  }

  //chan3
  irsend.sendNEC(0xFF20DF,32);
  while (chan3 > 0) {
    chan3 --;
    irsend.sendNEC(0xFF906F,32);
  }

  //chan4
  irsend.sendNEC(0xFF609F,32);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(0xFF906F,32);
  }

}

void q2()
{
  //chan1
  irsend.sendNEC(0xFFC03F,32);
  if(chan1 < 11)
  {
    while (chan1 < 11) {
      chan1 ++;
      irsend.sendNEC(0xFF708F,32);
    }
  }
  
  //chan2
  irsend.sendNEC(0xFFE01F,32);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(0xFF906F,32);
  }

  //chan3
  irsend.sendNEC(0xFF20DF,32);
  while (chan3 > 0) {
    chan3 --;
    irsend.sendNEC(0xFF906F,32);
  }

  //chan4
  irsend.sendNEC(0xFF609F,32);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(0xFF906F,32);
  }

}


void q3()
{
  irsend.sendNEC(0xFFC03F,32);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(0xFF708F,32);
  }
  
  irsend.sendNEC(0xFFE01F,32);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(0xFF906F,32);
  }

  irsend.sendNEC(0xFF20DF,32);
  while (chan3 < 11) {
    chan3 ++;
    irsend.sendNEC(0xFF708F,32);
  }

  irsend.sendNEC(0xFF609F,32);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(0xFF906F,32);
  }
}

void q4()
{
  irsend.sendNEC(0xFFC03F,32);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(0xFF708F,32);
  }
  
  irsend.sendNEC(0xFFE01F,32);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(0xFF906F,32);
  }

  irsend.sendNEC(0xFF20DF,32);
  while (chan3 < 11) {
    chan3 ++;
    irsend.sendNEC(0xFF708F,32);
  }

  irsend.sendNEC(0xFF609F,32);
  while (chan4 < 11) {
    chan4 ++;
    irsend.sendNEC(0xFF708F,32);
  }
}

void q5()
{
  irsend.sendNEC(0xFFC03F,32);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(0xFF708F,32);
  }
  
  irsend.sendNEC(0xFFE01F,32);
  if(chan2 < 5)
  {
    while (chan2 < 5) {
      chan2 ++;
      irsend.sendNEC(0xFF708F,32);
    }
  }
  else if(chan2 > 5)
  {
    while (chan2 > 5) {
      chan2 --;
      irsend.sendNEC(0xFF906F,32);
    }
  }

  irsend.sendNEC(0xFF20DF,32);
  while (chan3 < 11) {
    chan3 ++;
    irsend.sendNEC(0xFF708F,32);
  }

  irsend.sendNEC(0xFF609F,32);
  while (chan4 < 11) {
    chan4 ++;
    irsend.sendNEC(0xFF708F,32);
  }
}

void q6()
{
  irsend.sendNEC(0xFFC03F,32);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(0xFF708F,32);
  }
  
  irsend.sendNEC(0xFFE01F,32);
  while (chan2 < 11) {
    chan2 ++;
    irsend.sendNEC(0xFF708F,32);
  }

  irsend.sendNEC(0xFF20DF,32);
  while (chan3 < 11) {
    chan3 ++;
    irsend.sendNEC(0xFF708F,32);
  }

  irsend.sendNEC(0xFF609F,32);
  while (chan4 < 11) {
    chan4 ++;
    irsend.sendNEC(0xFF708F,32);
  }
}

void q7()
{
  irsend.sendNEC(0xFFC03F,32);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(0xFF708F,32);
  }
  
  irsend.sendNEC(0xFFE01F,32);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(0xFF906F,32);
  }

  irsend.sendNEC(0xFF20DF,32);
  while (chan3 < 11) {
    chan3 ++;
    irsend.sendNEC(0xFF708F,32);
  }

  irsend.sendNEC(0xFF609F,32);
  while (chan4 < 11) {
    chan4 ++;
    irsend.sendNEC(0xFF708F,32);
  }
}

void q8()
{
  irsend.sendNEC(0xFFC03F,32);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(0xFF708F,32);
  }
  
  irsend.sendNEC(0xFFE01F,32);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(0xFF906F,32);
  }

  irsend.sendNEC(0xFF20DF,32);
  while (chan3 < 11) {
    chan3 ++;
    irsend.sendNEC(0xFF708F,32);
  }

  irsend.sendNEC(0xFF609F,32);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(0xFF906F,32);
  }
}

void q9()
{
  irsend.sendNEC(0xFFC03F,32);
  while (chan1 < 11) {
    chan1 ++;
    irsend.sendNEC(0xFF708F,32);
  }
  
  irsend.sendNEC(0xFFE01F,32);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(0xFF906F,32);
  }

  irsend.sendNEC(0xFF20DF,32);
  while (chan3 > 0) {
    chan3 --;
    irsend.sendNEC(0xFF906F,32);
  }

  irsend.sendNEC(0xFF609F,32);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(0xFF906F,32);
  }
}

void q10()
{
  irsend.sendNEC(0xFFC03F,32);
  while (chan1 > 2) {
    chan1 --;
    irsend.sendNEC(0xFF906F,32);
  }
  
  irsend.sendNEC(0xFFE01F,32);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(0xFF906F,32);
  }

  irsend.sendNEC(0xFF20DF,32);
  while (chan3 > 0) {
    chan3 --;
    irsend.sendNEC(0xFF906F,32);
  }

  irsend.sendNEC(0xFF609F,32);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(0xFF906F,32);
  }
}

void q11()
{
  irsend.sendNEC(0xFFC03F,32);
  while (chan1 > 0) {
    chan1 --;
    irsend.sendNEC(0xFF906F,32);
  }
  irsend.sendNEC(0xFF906F,32);
  
  irsend.sendNEC(0xFFE01F,32);
  while (chan2 > 0) {
    chan2 --;
    irsend.sendNEC(0xFF906F,32);
  }

  irsend.sendNEC(0xFF20DF,32);
  while (chan3 > 0) {
    chan3 --;
    irsend.sendNEC(0xFF906F,32);
  }

  irsend.sendNEC(0xFF609F,32);
  while (chan4 > 0) {
    chan4 --;
    irsend.sendNEC(0xFF906F,32);
  }
}
