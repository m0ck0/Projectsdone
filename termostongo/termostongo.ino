#include <EEPROM.h>
#include <LiquidCrystal.h>
#include "DHT.h"
LiquidCrystal lcd(6,7,8,9,10,11);  
DHT dht(2, DHT22);

unsigned long previousMillis = 0;
const long interval = 2000;
#define relay 12
#define data 3
#define clk 4 
int State;
int LastState;  
float MaxTemp=0;
float MinTemp=100;
int MaxHum=0;
int MinHum=100;
float RealTemp;
float TargetTemp;
int RealHum;
float temp_hum_val[2] = {0};    
int MinTempEE;
int MaxTempEE;
int MinHumEE;
int MaxHumEE;

byte lamparica [8] ={    B01110,    B10001,    B10001,    B10001,    B01010,    B01110,    B01110,    B00100};
byte lamparicon [8] ={    B01110,    B11111,    B11111,    B11111,    B01110,    B01110,    B01010,    B00100};
byte termometru[8] = {    B00100,    B01010,    B01010,    B01110,    B01110,    B11111,    B11111,    B01110};
byte picagota[8] = {    B00100,    B00100,    B01010,    B01010,    B10001,    B10001,    B10001,    B01110,};
byte sadface[8] = {    B00000,    B11011,    B11011,    B00010,    B00001,    B01110,    B10001,    B00000,};



void setup() {
  EEPROM.read (1);
  Wire.begin();
  dht.begin();
  lcd.begin(16, 2);
  lcd.createChar(1,termometru);
  lcd.createChar(2,picagota);
  lcd.createChar(3,lamparica);
  lcd.createChar(4,lamparicon);
  lcd.createChar(5,sadface);
   pinMode (relay,OUTPUT);
   pinMode (clk,INPUT);
   pinMode (data,INPUT);   
   LastState = digitalRead(clk);   

}
 
void loop() {
  TargetTemp = EEPROM.read(1);
  lcd.setCursor(0, 0);
  lcd.write(1);
 if (RealTemp<10){lcd.print(" ");}
  lcd.print(RealTemp,1);
  lcd.setCursor(2, 0);
  lcd.setCursor(5, 0);
  lcd.print((char)223); 
  lcd.print("C");
  lcd.setCursor(8, 0);
  lcd.write(3);
  lcd.setCursor(9, 0);
if (TargetTemp<10){lcd.print(" ");}
  lcd.print(TargetTemp,1);  
  lcd.setCursor(13, 0);
  lcd.print((char)223); 
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.write(2);
if (RealHum<10){lcd.print(" ");}
  lcd.setCursor(2, 1);
  lcd.print(RealHum);
  lcd.setCursor(3, 1);
  lcd.setCursor(4, 1);
  lcd.print("%");
  lcd.setCursor(6, 1);
  encoder();
  relays();
 unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
   previousMillis = currentMillis;
  temphum();
  MinMax();
  }
  }

void encoder(){
   if (TargetTemp < 6) TargetTemp = 6;
   if (TargetTemp > 40) TargetTemp = 40;  
  State = digitalRead(clk);
   if (State != LastState){     
     if (digitalRead(data) != State) { 
       TargetTemp ++;
     } else {
       TargetTemp --;
     }
   } 
   LastState = State;
      EEPROM.write (1,TargetTemp);
}
  
void relays(){
  if (TargetTemp > RealTemp) {
    digitalWrite(relay,HIGH);
    lcd.setCursor(15, 0);
    lcd.write(4);
    } else {
    digitalWrite(relay,LOW);
    lcd.setCursor(15, 0);
    lcd.write(3);
    }
}
    
void temphum(){
  if(!dht.readTempAndHumidity(temp_hum_val)){
  RealHum = temp_hum_val[0];
  RealTemp = temp_hum_val[1];
    }
}

void MinMax(){
    if (RealHum<MinHum && RealHum!=100){
      MinHum=RealHum;
      EEPROM.write (2,MinHum);
    }
    if (RealHum>MaxHum && RealHum!=0){
      MaxHum=RealHum;
      EEPROM.write (3,MaxHum);
    }
    if (RealTemp<MinTemp && RealTemp!=100){
      MinTemp=RealTemp;
      EEPROM.write (4,MinTemp);
    }
    if (RealTemp>MaxTemp && RealTemp!=0){
      MaxTemp=RealTemp;
      EEPROM.write (5,MaxTemp);
    }
   }
