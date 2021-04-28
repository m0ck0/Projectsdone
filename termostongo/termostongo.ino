#include <EEPROM.h>
#include "DHT.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(6,7,8,9,10,11);  
#define DHTTYPE DHT11

int reading = 0;
int targetTemp =26;
int sensorPin = A0;
int relay =12;
int EncoderA  = 3;
int EncoderB  = 2;
//int Encoder_Switch = 4;
int Previous_Output;
int Count;
int realTemp=20;

byte lamparica [8] =
{
    B01110,
    B10001,
    B10001,
    B10001,
    B01010,
    B01110,
    B01110,
    B00100
};
byte lamparicon [8] =
{
    B01110,
    B11111,
    B11111,
    B11111,
    B01110,
    B01110,
    B01010,
    B00100
};
byte termometru[8] = 
{
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110
};

byte picagota[8] = 
{
    B00100,
    B00100,
    B01010,
    B01010,
    B10001,
    B10001,
    B10001,
    B01110,
};


void setup() {
  
  EEPROM.read (1);
  lcd.begin(16, 2);
  lcd.createChar(1,termometru);
  lcd.createChar(2,picagota);
  lcd.createChar(3,lamparica);
  lcd.createChar(4,lamparicon);
  pinMode (EncoderA, INPUT);
  pinMode (EncoderB, INPUT);
//  pinMode (Encoder_Switch, INPUT);
  pinMode(relay,OUTPUT);
  Previous_Output = digitalRead(EncoderA);

}
 
void loop() {
//  float h = dht.readHumidity();
//  float t = dht.readTemperature();
//  reading = analogRead(sensorPin);
//  int celsius = reading/2;
  targetTemp = EEPROM.read(1);
  
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.print(realTemp);
  lcd.setCursor(3, 0);
//  lcd.print((float)DHT11.temperature, 0);
//  lcd.print(t);
  lcd.setCursor(4, 0);
  lcd.print((char)223); 
  lcd.print("C");
  lcd.setCursor(7, 0);
  lcd.write(3);
  lcd.setCursor(8, 0);
  lcd.print(targetTemp);  
  lcd.setCursor(10, 0);
  lcd.print((char)223); 
  lcd.print("C");
  lcd.setCursor(14, 0);
if (targetTemp > realTemp) {
    digitalWrite(relay,HIGH);
    lcd.write(4);
  } else {
    digitalWrite(relay,LOW);
    lcd.write(3);
  }
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.setCursor(3, 1);
//  lcd.print(h);
//  lcd.print((float)DHT11.humidity, 0);
  lcd.setCursor(4, 1);
  lcd.print("%");
  lcd.setCursor(6, 0);
  encoder();
  delay(5);
  lcd.clear();
}

void encoder(){
   if (targetTemp < 6) targetTemp = 6;
   if (targetTemp > 36) targetTemp = 36;  
   if (digitalRead(EncoderA) != Previous_Output)
   { 
     if (digitalRead(EncoderB) != Previous_Output) 
     { 
       targetTemp ++;
     }
     else
     {
       targetTemp --;
      }
    
}
   Previous_Output = digitalRead(EncoderA);
EEPROM.write (1,targetTemp);
}
