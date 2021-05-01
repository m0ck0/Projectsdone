#include <EEPROM.h>
#include "DHT.h"
#include <LiquidCrystal.h>
LiquidCrystal lcd(6,7,8,9,10,11);  
DHT dht(2, DHT22);

float MaxTemp;
float MinTemp=100;
float MaxHum;
float MinHum=100;
float RealTemp;
float RealHum;

byte lamparica [8] ={    B01110,    B10001,    B10001,    B10001,    B01010,    B01110,    B01110,    B00100};
byte lamparicon [8] ={    B01110,    B11111,    B11111,    B11111,    B01110,    B01110,    B01010,    B00100};
byte termometru[8] = {    B00100,    B01010,    B01010,    B01110,    B01110,    B11111,    B11111,    B01110};
byte picagota[8] = {    B00100,    B00100,    B01010,    B01010,    B10001,    B10001,    B10001,    B01110,};
byte sadface[8] = {    B00000,    B11011,    B11011,    B00010,    B00001,    B01110,    B10001,    B00000,};

void setup() {
    Wire.begin();
    dht.begin();
    lcd.begin(20, 4);
    lcd.createChar(1,termometru);
    lcd.createChar(2,picagota);
    lcd.createChar(3,lamparica);
    lcd.createChar(4,lamparicon);
    lcd.createChar(5,sadface);
}

void loop() {
    float temp_hum_val[2] = {0};    
    
    if(!dht.readTempAndHumidity(temp_hum_val)){
    lcd.clear();
    lcd.setCursor(0, 0);   
    lcd.write(2); 
    lcd.setCursor(2, 0);   
    lcd.print(temp_hum_val[0],0);
    lcd.setCursor(4, 0);   
    lcd.print("%  ");
    lcd.setCursor(0, 1);   
    lcd.write(1); 
    lcd.setCursor(2, 1);   
    lcd.print(temp_hum_val[1],1);
    lcd.setCursor(6, 1);   
    lcd.print((char)223);
    lcd.print("C ");
    lcd.setCursor(0, 2);   
    lcd.write(2); 
    lcd.print("Min ");
    lcd.print(MinHum,0);
    lcd.print("%");
    lcd.print(" Max ");
    lcd.print(MaxHum,0);
    lcd.print("%");
    lcd.setCursor(0, 3);   
    lcd.write(1); 
    lcd.print("Min ");
    lcd.print(MinTemp,1);
    lcd.print((char)223);
    lcd.print(" Max ");
    lcd.print(MaxTemp,1);
    lcd.print((char)223);
    }    else{
    lcd.clear();
    lcd.setCursor(0, 1);   
    lcd.print("No veo nada,");
    lcd.setCursor(0, 2);   
    lcd.print("y es mui triste ");
    lcd.write(5); 
        }
    RealHum = temp_hum_val[0];
    RealTemp = temp_hum_val[1];
    delay(1500);
    MinMax();
    }

void MinMax(){
    if (RealHum > MaxHum) {
    MaxHum=RealHum;
    }
    if (RealHum < MinHum) {
    MinHum=RealHum;
    }
    if (RealTemp > MaxTemp) {
    MaxTemp=RealTemp;
    }
    if (RealTemp < MinTemp) {
    MinTemp=RealTemp;
    }
   }
