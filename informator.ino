Sa818+dfplayer mini+arduino nano

#include "SA818.h"
//Serial serial;
#include "timer-api.h"
//#include <SoftwareSerial.h>
//
// IO Definitions
//
#define PIN_AMP     10  // Direct control of output PA (need hardware mod)
#define PIN_HL      8  // High/Low power pin (Input - HIGH, OutputLow - Low)
#define PIN_MIC     11  // DAC pin for tone out
#define PIN_PTT     9  // PTT pin. This is active low.
#define PIN_PD      6  // Power Down pin. This need to start low, then be set high before programming.

//Comment to use hardware Serial instead
SoftwareSerial ser1(4,5);
SoftwareSerial ser(7,3);
#include <DFPlayer_Mini_Mp3.h>
//int vr = 18040; //7230
//int state = 0; // 0-do st, 1 - start;
//int dost = 920; //910
 int vr =14440; //7230
int state = 0; // 0-do st, 1 - start;
int dost = 920; //910
void setup()
{
  Serial.begin(9600);
  ser1.begin(9600);
  ser.begin(9600);
  
  pinMode(PIN_PTT, OUTPUT);
  pinMode(PIN_HL, OUTPUT);
  pinMode(2, INPUT);
  digitalWrite(PIN_PTT, HIGH);
  digitalWrite(PIN_HL, LOW);
  // Initialise Serial interface.
  pinMode(11, OUTPUT);
  inite();
  conf();
  //radio.begin();
  //radio.setConfig(SA_BANDWIDTH_12_5KHZ, "433.3500", "433.3500","0000", "0000", SA_SQUELCH_OFF);
  //radio.setPower(SA_POWER_LOW);
  //radio.setVolume(SA_VOLUME_DEFAULT);
  //radio.setFilters(SA_FILTER_OFF, SA_FILTER_OFF, SA_FILTER_OFF);
  pinMode(PIN_AMP, OUTPUT);
  pinMode(PIN_PD, OUTPUT);
  digitalWrite(PIN_PTT, HIGH);
  digitalWrite(PIN_PD, HIGH);
  mp3_set_serial(ser); // включаем передачу данных с DFPlayer mini mp3
  delay(100);               // задержка в 1ms для установки громкости
  mp3_set_volume(6);     // устанавливаем громкость от 0 до 30
  mp3_set_EQ(5);
  delay(2000);
digitalWrite(PIN_HL, LOW);
 send(1);
 delay(1000);
         digitalWrite(11, HIGH);
        delay(800);
        digitalWrite(11, LOW);
        delay(300);
    
 timer_init_ISR_1Hz(TIMER_DEFAULT);
        digitalWrite(11, HIGH);
        delay(200);
        digitalWrite(11, LOW);
        delay(300);
}

void loop()
{
  // Key the PTT on and off repeatedly.
  
    if (state == 0)
  {
    if (dost > 0)
    {
      if (dost == 899)
      {
        send(2);
        Serial.print("2");
      }
      if (dost == 880)
      {
        //send(22);
        Serial.print("22");
        /*
        digitalWrite(PIN_PTT, LOW);
       delay(1000);
        mp3_play(22);
   
        delay(62000);
       mp3_stop();
  
       delay(500);
       digitalWrite(PIN_PTT, HIGH);
       */
       send(22);
      }
       if(dost == 600)
      {
        send(3);
        Serial.print("3");
      }
    
      if (dost == 300)
      {
        send(4);
        Serial.print("4");

      }
      
    }
    else
    {
      state = 1;
      send(5);
      delay(2000);
      send(5);
      delay(2000);
      send(5);
      delay(2000);
      digitalWrite(11, HIGH);
      delay(3000);
      digitalWrite(11, LOW);
      
    }
  }
  if (state == 1)
  {
    if(vr>0)
    {
      if(vr==18000)
      {
        send(23);
      }
      if(vr==14400)
      {
        send(24);
      }
      if(vr==10800)
      {
        send(25);
      }
      if(vr==7200)
      {
        send(9);
      }
      if(vr==3600)
      {
        send(10);
      }
      if(vr==1800)
      {
        send(11);
      }
      if(vr==900)
      {
        send(12);
      }
       if(vr==600)
      {
        send(13);
      }
      if(vr==300)
      {
        send(14);
      }
      if(vr==60)
      {
        send(15);
      }
    }
    else
    {
       Serial.print("22");
       digitalWrite(PIN_PTT, LOW);
       delay(1000);
       mp3_play(16);
       delay(8000);
       mp3_stop();
       delay(500);
       digitalWrite(PIN_PTT, HIGH);
       delay(1000);

      timer_stop_ISR(TIMER_DEFAULT);
      while(true)
      {
        digitalWrite(11, HIGH);
        delay(1000);
        digitalWrite(11, LOW);
        delay(2000);
      }
    }

  }
    
 

}



void timer_handle_interrupts(int timer) {
  //Serial.println(analogRead(A2));
  if(state==0)
  {
    if(dost>0)
    {
      dost--;
    }
    else
    {
      state=1;
      
    }
  }
  if(state==1)
  {
    if(vr>0)
    {
      vr--;
      
    }
    else
    {
      
    }
  }
  
}
void send(int num)
{
  digitalWrite(PIN_PTT, LOW);
  delay(1000);
  mp3_play(num);
  delay(100);
  while(analogRead(A2)<500)
  {
    delay(500);
  }
  //delay(2000);
  mp3_stop();
  Serial.println(dost);
  delay(500);
  digitalWrite(PIN_PTT, HIGH);
}
void inite()
{
  //ser1.print("AT\r\n");
  //delay(100);
  ser1.print("AT+DMOCONNECT\r\n");
  delay(200);
  Serial.println(ser1.read());
}
void conf()
{
  ser1.print("AT+DMOSETGROUP=");
  ser1.print(1); // 0/1
  ser1.print(",");
  ser1.print("433.3500");//134-174/400-480, format to 415.1250
  ser1.print(",");
  ser1.print("433.3500");//format to 415.1250
  ser1.print(",");
  ser1.print("0000");//format to 0000
  ser1.print(",");
  ser1.print("0"); // <= 8
  ser1.print(",");
  ser1.print("0000");
  ser1.print("\r\n");
  Serial.println(ser1.read());
}
