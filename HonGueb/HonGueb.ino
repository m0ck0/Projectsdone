#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include <DHT.h>
#include <EEPROM.h>
//#include <LiquidCrystal_I2C.h>

DHT dht (D5, DHT22);
//LiquidCrystal_I2C lcd(0x27, 20, 4);

const char* ssid = "Nichoo";
const char* password = "milanesas";

const unsigned long Minutos = 60 * 1000UL;
byte HumidiOn = 3; 
byte HumidiWait = 15;
byte AirOn = 2; 
byte AirWait = 5;
unsigned long previousMillis = 0;
unsigned long prevMillis = 0;
const long interval = 2000;
const byte boton = 5;
const byte Calentador = 12;
const byte Humidi = 13;
const byte Aire = 14;
const byte AireW = 15;
byte MaxTemp = 0;
byte MinTemp = 100;
byte MaxHum = 0;
byte MinHum = 100;
byte Display = 0;
byte TargetTemp = 22;
byte LastTemp;
int RealHum;
float temp_hum_val[2] = {0};
float RealTemp;

/*
byte termometru[8] = {  B00100,  B01010,  B01010,  B01110,  B01110,  B11111,  B11111,  B01110};
byte picagota[8] =   {  B00100,  B00100,  B01010,  B01010,  B10001,  B10001,  B10001,  B01110};
byte lamparica [8] = {  B01110,  B10001,  B10001,  B10001,  B01010,  B01110,  B01110,  B00100};
byte lamparicon[8] = {  B01110,  B11111,  B11111,  B11111,  B01110,  B01110,  B01010,  B00100};
byte sadface[8] =    {  B00000,  B11011,  B11011,  B00010,  B00001,  B01110,  B10001,  B00000};
byte downarrow[8] =  {  B00000,  B00000,  B00000,  B01110,  B01110,  B11111,  B01110,  B00100};
byte uparrow[8] =    {  B00100,  B01110,  B11111,  B01110,  B01110,  B00000,  B00000,  B00000};
byte gotallena[8] =  {  B00100,  B00100,  B01110,  B01110,  B11111,  B11111,  B11111,  B01110};
*/
AsyncWebServer server(80);

String processor(const String& var) {
  if (var == "Calentador") {
    if (digitalRead(Calentador)) {
      /*      ledState = "ON";
          }else{
            ledState = "OFF";
      */
    }
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
  else if (var == "HumO") {
    return String(HumidiOn);
  }
  else if (var == "HumW") {
    return String(HumidiWait);
  }
  else if (var == "AirO") {
    return String(AirOn);
  }
  else if (var == "AirW") {
    return String(AirWait);
  }
  return String();

}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); 
  Serial.begin(115200);
  Wire.begin();
  EEPROM.begin(512);
  dht.begin();
  /*
  lcd.init();
  lcd.backlight();
  lcd.createChar(1, termometru);
  lcd.createChar(2, picagota);
  lcd.createChar(3, lamparica);
  lcd.createChar(4, lamparicon);
  lcd.createChar(5, sadface);
  lcd.createChar(6, downarrow);
  lcd.createChar(7, uparrow);
  lcd.createChar(8, gotallena);
  */
  pinMode (Calentador, OUTPUT);
  pinMode (Humidi, OUTPUT);
  pinMode (Aire, OUTPUT);
  pinMode (AireW, OUTPUT);
  pinMode (boton, INPUT);
  TargetTemp = EEPROM.read(1);
  MinTemp = EEPROM.read(2);
  MaxTemp = EEPROM.read(3);
  MinHum = EEPROM.read(4);
  MaxHum = EEPROM.read(5);
  HumidiOn = EEPROM.read(6);
  HumidiWait = EEPROM.read(7);
  AirOn = EEPROM.read(8);
  AirWait = EEPROM.read(9);

  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/menos", HTTP_GET, [](AsyncWebServerRequest * request) {
    TargetTemp --;
    EEPROM.write(1, TargetTemp);
    EEPROM.commit();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/mas", HTTP_GET, [](AsyncWebServerRequest * request) {
    TargetTemp ++;
    EEPROM.write(1, TargetTemp);
    EEPROM.commit();
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
  server.on("/menoshumon", HTTP_GET, [](AsyncWebServerRequest * request) {
    HumidiOn --;
    EEPROM.write(6, HumidiOn);
    EEPROM.commit();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/mashumon", HTTP_GET, [](AsyncWebServerRequest * request) {
    HumidiOn ++;
    EEPROM.write(6, HumidiOn);
  EEPROM.commit();
  request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/menoshumw", HTTP_GET, [](AsyncWebServerRequest * request) {
    HumidiWait --;
    EEPROM.write(7, HumidiWait);
    EEPROM.commit();
  request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/mashumw", HTTP_GET, [](AsyncWebServerRequest * request) {
    HumidiWait ++;
    EEPROM.write(7, HumidiWait);
    EEPROM.commit();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/menosairon", HTTP_GET, [](AsyncWebServerRequest * request) {
    AirOn --;
    EEPROM.write(8, AirOn);
    EEPROM.commit();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/masairon", HTTP_GET, [](AsyncWebServerRequest * request) {
    AirOn ++;
    EEPROM.write(8, AirOn);
  EEPROM.commit();
  request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/menosairw", HTTP_GET, [](AsyncWebServerRequest * request) {
    AirWait --;
    EEPROM.write(9, AirWait);
    EEPROM.commit();
  request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/masairw", HTTP_GET, [](AsyncWebServerRequest * request) {
    AirWait ++;
    EEPROM.write(9, AirWait);
    EEPROM.commit();
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest * request) {
        EEPROM.write(2, RealTemp);
        EEPROM.write(3, RealTemp);
        EEPROM.write(4, RealHum);
        EEPROM.write(5, RealHum);
        EEPROM.write(6, HumidiOn);
        EEPROM.write(7, HumidiWait);
        EEPROM.write(8, AirOn);
        EEPROM.write(9, AirWait);
        EEPROM.commit();
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
  server.on("/GetMinHum", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(MinHum).c_str());
  });
  server.on("/GetMaxHum", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(MaxHum).c_str());
  });
  server.on("/GetHumidiOn", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(HumidiOn).c_str());
  });
  server.on("/GetHumidiWaiT", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(HumidiWait).c_str());
  });
  server.on("/GetAirOn", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(AirOn).c_str());
  });
  server.on("/GetAirWaiT", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(AirWait).c_str());
  });
/*
  server.on("/bg", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/bg.png", "image/png");
  });
  server.on("/grados", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/grados.png", "image/png");
  });
  server.on("/heat", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/heat.png", "image/png");
  });
  server.on("/hume", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/hume.png", "image/png");
  });
  server.on("/humi", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/humi.png", "image/png");
  });
  server.on("/max", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/max.png", "image/png");
  });
  server.on("/min", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/min.png", "image/png");
  });
  server.on("/minus", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/minus.png", "image/png");
  });
  server.on("/percen", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/percen.png", "image/png");
  });
  server.on("/plus", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/plus.png", "image/png");
  });
  server.on("/poweroff", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/poweroff.png", "image/png");
  });
  server.on("/poweron", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/poweron.png", "image/png");
  });
  server.on("/termo", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/termo.png", "image/png");
  });
  server.on("/termos", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/termos.png", "image/png");
  });
*/
  server.begin();
}


void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    temphum();
    MinMax();
  }
  /*    if (digitalRead(reset) == LOW) {
        delay(400);
        EEPROM.write(2, RealTemp);
        EEPROM.write(3, RealTemp);
        EEPROM.write(4, RealHum);
        EEPROM.write(5, RealHum);
        EEPROM.write(6, HumidiOn);
        EEPROM.write(7, HumidiWait);
        EEPROM.commit();
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
  */
  calentador();
  timerhumi();
}

void calentador() {
  if ((temp_hum_val[1] != 0) && (TargetTemp >= RealTemp) && (LastTemp < RealTemp)) {
    digitalWrite(Calentador, HIGH);
  } else if ((temp_hum_val[1] != 0) && ((TargetTemp - LastTemp) >= 1 ) && (LastTemp > RealTemp)) {
    digitalWrite(Calentador, HIGH);
  } else {
    digitalWrite(Calentador, LOW);
  }
}

void timerhumi() {
  unsigned long curMillis = millis();
  if ((digitalRead(Aire)) && (!digitalRead(Humidi)) && (curMillis - prevMillis >= (AirOn*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Aire, LOW);
    digitalWrite(AireW, HIGH);
  }  else if ((digitalRead(AireW)) && (curMillis - prevMillis >= (AirWait*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Humidi, HIGH);
    digitalWrite(Aire, HIGH);
  }  else if ((digitalRead(Aire)) && (digitalRead(Humidi)) && (curMillis - prevMillis >= (HumidiOn*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Humidi, LOW);
    digitalWrite(Aire, LOW);
  }  else if ((!digitalRead(Aire)) && (!digitalRead(Humidi)) && (curMillis - prevMillis >= (HumidiWait*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Aire, HIGH);
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
    EEPROM.commit();
  }
  if (RealTemp > MaxTemp) {
    MaxTemp = RealTemp;
    EEPROM.write(3, MaxTemp);
    EEPROM.commit();
  }
  if (RealHum < MinHum && RealTemp != 0) {
    MinHum = RealHum;
    EEPROM.write(4, MinHum);
    EEPROM.commit();
  }
  if (RealHum > MaxHum) {
    MaxHum = RealHum;
    EEPROM.write(5, MaxHum);
    EEPROM.commit();
  }
}

/*
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
*/
void error() {
/* 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No veo nada,  ");
  lcd.write(5);
  lcd.setCursor(0, 1);
  lcd.print("y es mui triste");
  delay(1000);
  lcd.clear();
*/
}
