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
#define boton 5
//#define reset 13
int State;
int LastState;  
int MaxTemp=0;
int MinTemp=100;
int MaxHum=0;
int MinHum=100;
float RealTemp;
int TargetTemp;
int TempInt;
int RealHum;
int Display=0;
float temp_hum_val[2] = {0};    


byte lamparica [8] = {  B01110,  B10001,  B10001,  B10001,  B01010,  B01110,  B01110,  B00100};
byte lamparicon [8] ={  B01110,  B11111,  B11111,  B11111,  B01110,  B01110,  B01010,  B00100};
byte termometru[8] = {  B00100,  B01010,  B01010,  B01110,  B01110,  B11111,  B11111,  B01110};
byte picagota[8] =   {  B00100,  B00100,  B01010,  B01010,  B10001,  B10001,  B10001,  B01110,};
byte sadface[8] =    {  B00000,  B11011,  B11011,  B00010,  B00001,  B01110,  B10001,  B00000,};
byte downarrow[8] =  {  B00000,  B00000,  B00000,  B01110,  B01110,  B11111,  B01110,  B00100};
byte uparrow[8] =    {  B00100,  B01110,  B11111,  B01110,  B01110,  B00000,  B00000,  B00000};

void setup() {
  EEPROM.read(1);
ccc  Wire.begin();
  dht.begin();
  lcd.begin(16, 2);
  lcd.createChar(1,termometru);
  lcd.createChar(2,picagota);
  lcd.createChar(3,lamparica);
  lcd.createChar(4,lamparicon);
  lcd.createChar(5,sadface);
  lcd.createChar(6,downarrow);
  lcd.createChar(7,uparrow);
  pinMode (relay,OUTPUT);
  pinMode (clk,INPUT);
  pinMode (data,INPUT);   
  pinMode (boton,INPUT);
//  pinMode (reset,INPUT);
//  digitalWrite (reset, HIGH);
  digitalWrite (boton, HIGH);
  LastState = digitalRead(clk);   
  MinTemp=EEPROM.read(2);
  MaxTemp=EEPROM.read(3);
  MinHum=EEPROM.read(4);
  MaxHum=EEPROM.read(5); 
  TargetTemp=EEPROM.read(6);
   }
 
void loop() {
   TempInt=RealTemp;

//  if (digitalRead(reset) == LOW){
//    EEPROM.write(2,RealTemp);
//    EEPROM.write(3,RealTemp);
//    EEPROM.write(4,RealHum);
//    EEPROM.write(5,RealHum);
//  }
  if (digitalRead(boton) == LOW){
    delay(500);

    Display=Display+1;
    if (Display>2){
      lcd.clear();
      Display=1;
    }
    switch (Display) {
      case 1: {mainpage();}
      case 2: {logpage();}
    }
  }
  if (RealTemp==0) {error();}
  encoder();
  mainpage();
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
   EEPROM.write (6,TargetTemp);
   
   }
  
void relays(){
  if (TargetTemp > TempInt) {
    digitalWrite(relay,HIGH);
    } else {
    digitalWrite(relay,LOW);
    }
}

void relayicon(){
  if (TargetTemp > RealTemp) {
    lcd.setCursor(15, 0);
    lcd.write(4);
    } else {
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
    if (RealTemp<MinTemp){
      MinTemp=RealTemp;
      EEPROM.write (2,MinTemp);
    }
    if (RealTemp>MaxTemp){
      MaxTemp=RealTemp;
      EEPROM.write (3,MaxTemp);
    }
    if (RealHum<MinHum){
      MinHum=RealHum;
      EEPROM.write (4,MinHum);
    }
    if (RealHum>MaxHum){
      MaxHum=RealHum;
      EEPROM.write (5,MaxHum);
    }
   }
  
void logpage(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.write(6);
 if (RealTemp<10){lcd.print(" ");}
  lcd.print(EEPROM.read(2));
  lcd.print((char)223); 
  lcd.print("C  ");
  lcd.write(7);
 if (RealTemp<10){lcd.print(" ");}
  lcd.print(EEPROM.read(3));
  lcd.print((char)223); 
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.write(6);
if (RealHum<10){lcd.print(" ");}
  lcd.print(EEPROM.read(4));
  lcd.print("%   ");
  lcd.write(7);
if (RealHum<10){lcd.print(" ");}
  lcd.print(EEPROM.read(5));
  lcd.print("%");
  delay(4000);
  lcd.clear();
}

void error(){
    lcd.clear();
    lcd.setCursor(0, 0);   
    lcd.print("No veo nada,  ");
    lcd.write(5); 
    lcd.setCursor(0, 1);   
    lcd.print("y es mui triste");
    delay(1000);
    lcd.clear();
}

void mainpage(){
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
  lcd.print(".0");
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
  lcd.setCursor(9, 1);
  lcd.print(EEPROM.read(6));
  lcd.print((char)223); 
  lcd.print("C");
  relayicon();
}
