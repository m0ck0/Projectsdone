#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <DHT.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
DHT dht(2, DHT22);
LiquidCrystal lcd(6, 7, 11, 8, 9, 10);

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
unsigned long previousMillis = 0;
unsigned long prevMillis = 0;
const unsigned long HumidiOn = 1 * 60 * 1000UL;
const unsigned long HumidiWait = 15 * 60 * 1000UL;
const long interval = 2000;
const byte boton = 5;
const byte Calentador = 12;
const byte Humidi = 13;
const byte reset = 14;
byte MaxTemp = 0;
byte MinTemp = 100;
byte MaxHum = 0;
byte MinHum = 100;
byte Display = 0;
byte TargetTemp;
byte LastTemp;
int RealHum;
float temp_hum_val[2] = {0};
float RealTemp;

byte termometru[8] = {  B00100,  B01010,  B01010,  B01110,  B01110,  B11111,  B11111,  B01110};
byte picagota[8] =   {  B00100,  B00100,  B01010,  B01010,  B10001,  B10001,  B10001,  B01110};
byte lamparica [8] = {  B01110,  B10001,  B10001,  B10001,  B01010,  B01110,  B01110,  B00100};
byte lamparicon[8] = {  B01110,  B11111,  B11111,  B11111,  B01110,  B01110,  B01010,  B00100};
byte sadface[8] =    {  B00000,  B11011,  B11011,  B00010,  B00001,  B01110,  B10001,  B00000};
byte downarrow[8] =  {  B00000,  B00000,  B00000,  B01110,  B01110,  B11111,  B01110,  B00100};
byte uparrow[8] =    {  B00100,  B01110,  B11111,  B01110,  B01110,  B00000,  B00000,  B00000};
byte gotallena[8] =  {  B00100,  B00100,  B01110,  B01110,  B11111,  B11111,  B11111,  B01110};

AsyncWebServer server(80);

String processor(const String& var) {
  if (var == "Calentador") {
    if (digitalRead(Calentador)) {
      /*      ledState = "ON";
          }else{
            ledState = "OFF";
      */}
    }
    else if (var == "RealTemp") {
      return String(RealTemp);
    }
    else if (var == "MinT") {
      return String(MinTemp);
    }
    else if (var == "MaxT") {
      return String(MaxTemp);
    }
    else if (var == "TargetTemp") {
      return String(TargetTemp);
    }
    else if (var == "RealHum") {
      return String(RealHum);
    }
    else if (var == "MinH") {
      return String(MinHum);
    }
    else if (var == "MaxH") {
      return String(MaxHum);
    }
    return String();
}

  void setup() {
    Wire.begin();
    dht.begin();
	lcd.begin(16, 2);
	lcd.createChar(1, termometru);
	lcd.createChar(2, picagota);
	lcd.createChar(3, lamparica);
	lcd.createChar(4, lamparicon);
	lcd.createChar(5, sadface);
	lcd.createChar(6, downarrow);
	lcd.createChar(7, uparrow);
	lcd.createChar(8, gotallena);
    pinMode (Calentador, OUTPUT);
    pinMode (Humidi, OUTPUT);
	  pinMode (boton, INPUT);
    pinMode (reset, INPUT);
    digitalWrite (reset, HIGH);
    digitalWrite (boton, HIGH);
  TargetTemp = EEPROM.read(1);
  MinTemp = EEPROM.read(2);
  MaxTemp = EEPROM.read(3);
  MinHum = EEPROM.read(4);
  MaxHum = EEPROM.read(5);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
    }
    server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send(SPIFFS, "/style.css", "text/css");
    });
    server.on("/menos", HTTP_GET, [](AsyncWebServerRequest * request) {
      TargetTemp --;
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });
    server.on("/mas", HTTP_GET, [](AsyncWebServerRequest * request) {
      TargetTemp ++;
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });
    server.on("/on", HTTP_GET, [](AsyncWebServerRequest * request) {
      digitalWrite(Humidi, HIGH);
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });
    server.on("/off", HTTP_GET, [](AsyncWebServerRequest * request) {
      digitalWrite(Humidi, LOW);
      request->send(SPIFFS, "/index.html", String(), false, processor);
    });
    server.on("/GetRealTemp", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send_P(200, "text/plain", String(RealTemp).c_str());
    });
    server.on("/GetMinTemp", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send_P(200, "text/plain", String(MinTemp).c_str());
    });
    server.on("/GetMaxTemp", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send_P(200, "text/plain", String(MaxTemp).c_str());
    });
    server.on("/GetTargetTemp", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send_P(200, "text/plain", String(TargetTemp).c_str());
    });
    server.on("/GetRealHum", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send_P(200, "text/plain", String(RealHum).c_str());
    });
    server.on("/GetMinHun", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send_P(200, "text/plain", String(MinHum).c_str());
    });
    server.on("/GetMaxHum", HTTP_GET, [](AsyncWebServerRequest * request) {
      request->send_P(200, "text/plain", String(MaxHum).c_str());
    });
    server.begin();
 }

void loop() {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      temphum();
      MinMax();
    }
  if (digitalRead(reset) == LOW) {
    delay(400);
    EEPROM.write(2, RealTemp);
    EEPROM.write(3, RealTemp);
    EEPROM.write(4, RealHum);
    EEPROM.write(5, RealHum);
  }
  if (digitalRead(boton) == LOW) {
    delay(400);
    Display = Display + 1;
    if (Display > 2) {
      lcd.clear();
      Display = 1;
    }
    switch (Display) {
      case 1: {
          mainpage();
        }
      case 2: {
          logpage();
        }
    }
  }   
   mainpage();
   calentador();
   timerhumi();
}
  
void calentador() {
  if ((temp_hum_val[1] != 0) && (TargetTemp >= RealTemp) && (LastTemp < RealTemp)) {
  digitalWrite(Calentador, HIGH);
  } else if ((temp_hum_val[1] != 0) && ((TargetTemp - LastTemp) >=1 ) && (LastTemp > RealTemp)){
    digitalWrite(Calentador, HIGH);
  } else { 
  digitalWrite(Calentador, LOW);
    }
} 

void timerhumi() {
  unsigned long curMillis = millis();
  if ((digitalRead(Humidi)) && (curMillis - prevMillis >= HumidiOn)) {
    prevMillis = curMillis;
    digitalWrite(Humidi, LOW);
  }  else if ((!digitalRead(Humidi)) && (curMillis - prevMillis >= HumidiWait)) {
    prevMillis = curMillis;
    digitalWrite(Humidi, HIGH);
  }
}

void temphum() {
  LastTemp = RealTemp;
  if (!dht.readTempAndHumidity(temp_hum_val)) {
    RealHum = temp_hum_val[0];
    RealTemp = temp_hum_val[1];
  } else {
    error();
  }
}

void MinMax() {
  if (RealTemp < MinTemp && RealTemp != 0) {
    MinTemp = RealTemp;
    EEPROM.write(2, MinTemp);
  }
  if (RealTemp > MaxTemp) {
    MaxTemp = RealTemp;
    EEPROM.write(3, MaxTemp);
  }
  if (RealHum < MinHum && RealTemp != 0) {
    MinHum = RealHum;
    EEPROM.write(4, MinHum);
  }
  if (RealHum > MaxHum) {
    MaxHum = RealHum;
    EEPROM.write(5, MaxHum);
  }
}

void error() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No veo nada,  ");
  lcd.write(5);
  lcd.setCursor(0, 1);
  lcd.print("y es mui triste");
  delay(1000);
  lcd.clear();
}
  
void mainpage() {
  lcd.setCursor(0, 0);
  lcd.write(1);
  if (RealTemp < 10) {
    lcd.print(" ");
  }
  lcd.print(temp_hum_val[1], 1);
  //  lcd.print(RealTemp,1);
  lcd.setCursor(2, 0);
  lcd.setCursor(5, 0);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(8, 0);
  lcd.write(3);
  lcd.setCursor(9, 0);
  if (TargetTemp < 10) {
    lcd.print(" ");
  }
  lcd.print(TargetTemp, 1);
  lcd.print(".0");
  lcd.setCursor(13, 0);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.write(2);
  if (RealHum < 10) {
    lcd.print(" ");
  }
  lcd.setCursor(2, 1);
  lcd.print(RealHum);
  lcd.setCursor(3, 1);
  lcd.setCursor(4, 1);
  lcd.print("%");
  lcd.setCursor(8, 1);
  icons();
}

void logpage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(1);
  lcd.write(6);
  if (MinTemp < 10) {
    lcd.print(" ");
  }
  lcd.print(EEPROM.read(2));
  lcd.print((char)223);
  lcd.print("C  ");
  lcd.write(7);
  if (MaxTemp < 10) {
    lcd.print(" ");
  }
  lcd.print(EEPROM.read(3));
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.write(2);
  lcd.write(6);
  if (MinHum < 10) {
    lcd.print(" ");
  }
  lcd.print(EEPROM.read(4));
  lcd.print("%   ");
  lcd.write(7);
  if (MaxHum < 10) {
    lcd.print(" ");
  }
  lcd.print(EEPROM.read(5));
  lcd.print("%");
  delay(4000);
  lcd.clear();
}

void icons() {
  if (digitalRead(Calentador)) {
    lcd.setCursor(15, 0);
    lcd.write(4);
  } else {
    lcd.setCursor(15, 0);
    lcd.write(3);
  }

  if (digitalRead(Humidi)) {
    lcd.setCursor(7, 1);
    lcd.write(8);
  } else {
    lcd.setCursor(7, 1);
    lcd.write(2);
  }
}