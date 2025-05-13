#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <Wire.h>
#include <EEPROM.h>
#include <OneWire.h>                
#include <DallasTemperature.h>
const char* ssid = "Nichoo";
const char* password = "milanesas";

const unsigned long Minutos = 60 * 1000UL;
byte HumidiOn = 1; 
byte HumidiWait = 1;
byte AirOn = 1; 
byte AirWait = 1;
unsigned long previousMillis = 0;
unsigned long prevMillis = 0;
const long interval = 6000;
const byte Calentador = D4;
const byte Humidi = D5;
const byte Aire = D6;
const byte AireOut = D7;
const byte ONE_WIRE_BUS = D3;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire); //Se declara una variable u objeto para nuestro sensor
byte AireW = 0;
String HumidiStatus = "patata";
String CalentadorStatus = "nose";
byte MaxTemp = 0;
byte MinTemp = 100;
//byte MaxHum = 0;
//byte MinHum = 100;
byte TargetTemp = 22;
//int RealHum;
//float temp_hum_val[2] = {0};
float temp;

AsyncWebServer server(80);

String processor(const String& var) {
  if (var == "HumidiStatus") {
   return String(HumidiStatus);
  }
  else if (var == "CalentadorStatus") {
   return String(CalentadorStatus);
  }
  else if (var == "temp") {
    return String(temp);
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
// else if (var == "RealHum") {
//    return String(RealHum);
// }
// else if (var == "MinH") {
//   return String(MinHum);
// }
// else if (var == "MaxH") {
//   return String(MaxHum);
// }
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
  Serial.begin(115200);
  EEPROM.begin(512);
  Wire.begin();
  sensors.begin();
//  dht.begin();
  pinMode (Calentador, OUTPUT);
  pinMode (Humidi, OUTPUT);
  pinMode (Aire, OUTPUT);
  pinMode (AireOut, OUTPUT);
  TargetTemp = EEPROM.read(1);
  MinTemp = EEPROM.read(2);
  MaxTemp = EEPROM.read(3);
//  MinHum = EEPROM.read(4);
//  MaxHum = EEPROM.read(5);
  HumidiOn = EEPROM.read(6);
  HumidiWait = EEPROM.read(7);
  AirOn = EEPROM.read(8);
  AirWait = EEPROM.read(9);

  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
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
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(LittleFS, "/style.css", "text/css");
  });
  server.on("/menos", HTTP_GET, [](AsyncWebServerRequest * request) {
    TargetTemp --;
    EEPROM.write(1, TargetTemp);
    EEPROM.commit();
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/mas", HTTP_GET, [](AsyncWebServerRequest * request) {
    TargetTemp ++;
    EEPROM.write(1, TargetTemp);
    EEPROM.commit();
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(Humidi, HIGH);
    digitalWrite(Aire, HIGH);
    HumidiStatus = "On";
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(Humidi, LOW);
    digitalWrite(Aire, LOW);
    HumidiStatus = "Off";
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
   server.on("/menoshumon", HTTP_GET, [](AsyncWebServerRequest * request) {
    HumidiOn --;
    EEPROM.write(6, HumidiOn);
    EEPROM.commit();
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/mashumon", HTTP_GET, [](AsyncWebServerRequest * request) {
    HumidiOn ++;
    EEPROM.write(6, HumidiOn);
  EEPROM.commit();
  request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/menoshumw", HTTP_GET, [](AsyncWebServerRequest * request) {
    HumidiWait --;
    EEPROM.write(7, HumidiWait);
    EEPROM.commit();
  request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/mashumw", HTTP_GET, [](AsyncWebServerRequest * request) {
    HumidiWait ++;
    EEPROM.write(7, HumidiWait);
    EEPROM.commit();
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/menosairon", HTTP_GET, [](AsyncWebServerRequest * request) {
    AirOn --;
    EEPROM.write(8, AirOn);
    EEPROM.commit();
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/masairon", HTTP_GET, [](AsyncWebServerRequest * request) {
    AirOn ++;
    EEPROM.write(8, AirOn);
  EEPROM.commit();
  request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/menosairw", HTTP_GET, [](AsyncWebServerRequest * request) {
    AirWait --;
    EEPROM.write(9, AirWait);
    EEPROM.commit();
  request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/masairw", HTTP_GET, [](AsyncWebServerRequest * request) {
    AirWait ++;
    EEPROM.write(9, AirWait);
    EEPROM.commit();
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
 server.on("/reset", HTTP_GET, [](AsyncWebServerRequest * request) {
        EEPROM.write(2, temp);
        EEPROM.write(3, temp);
//       EEPROM.write(4, RealHum);
//       EEPROM.write(5, RealHum);
        EEPROM.commit();
        MinTemp = temp;
        MaxTemp = temp;
//       MinHum = RealHum;
//       MaxHum = RealHum;
        Serial.println("Reset");
    request->send(LittleFS, "/index.html", String(), false, processor);
  });
  server.on("/Gettemp", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(temp).c_str());
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
 // server.on("/GetRealHum", HTTP_GET, [](AsyncWebServerRequest * request) {
 //   request->send_P(200, "text/plain", String(RealHum).c_str());
 // });
 // server.on("/GetMinHum", HTTP_GET, [](AsyncWebServerRequest * request) {
 //   request->send_P(200, "text/plain", String(MinHum).c_str());
 // });
 // server.on("/GetMaxHum", HTTP_GET, [](AsyncWebServerRequest * request) {
 //   request->send_P(200, "text/plain", String(MaxHum).c_str());
 // });
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
  server.on("/GetCalentador", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(CalentadorStatus).c_str());
  });  
//  server.on("/GetHumidi", HTTP_GET, [](AsyncWebServerRequest * request) {
//    request->send_P(200, "text/plain", String(HumidiStatus).c_str());
//  });  
  server.begin();
}


void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
   previousMillis = currentMillis;
   temphum();
   MinMax();
  }
  calentador();
  timerhumi();

}

void calentador() {
  if (temp<0 && (digitalRead(Calentador))){
  digitalWrite(Calentador, LOW);
  CalentadorStatus = "Off";
  Serial.println("CalentOff (no data)");
    } 
  else if (TargetTemp >= temp && (!digitalRead(Calentador)) && temp>0) {
    digitalWrite(Calentador, HIGH);
    CalentadorStatus = "On";
    Serial.println("CalentOn");
  }
 else if (TargetTemp < temp && (digitalRead(Calentador)) && temp>0) {
  digitalWrite(Calentador, LOW);
	CalentadorStatus = "Off";
  Serial.println("CalentOff");
	}
}

void timerhumi() {
  unsigned long curMillis = millis();
  if ((digitalRead(Aire)) && (!digitalRead(Humidi)) && (AireW==0) && (curMillis - prevMillis >= (AirOn*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Aire, LOW);
    digitalWrite(AireOut, LOW);
    AireW = 1;
    HumidiStatus = "WaitAir";
    Serial.println("WaitAir");
  }  else if ((!digitalRead(Aire)) && (!digitalRead(Humidi)) && (AireW==1) && (curMillis - prevMillis >= (AirWait*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Humidi, HIGH);
    digitalWrite(Aire, HIGH);
    AireW = 0;
	HumidiStatus = "Humiding";
    Serial.println("Humiding");
  }  else if ((digitalRead(Aire)) && (digitalRead(Humidi)) && (AireW==0) && (curMillis - prevMillis >= (HumidiOn*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Humidi, LOW);
    digitalWrite(Aire, LOW);
    AireW = 0;
	HumidiStatus = "Waiting";
    Serial.println("Waiting");
  }  else if ((!digitalRead(Aire)) && (!digitalRead(Humidi)) && (AireW==0) && (curMillis - prevMillis >= (HumidiWait*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Aire, HIGH);
    digitalWrite(AireOut, HIGH);
    HumidiStatus = "Airing";
    AireW = 0;
    Serial.println("Airing");
  }
}

void temphum() {
sensors.requestTemperatures();   //Se envía el comando para leer la temperatura
float temp = sensors.getTempCByIndex(0); //Se obtiene la temperatura en ºC
//  if (!dht.readTempAndHumidity(temp_hum_val)) {
//  RealTemp = temp_hum_val[1];
//  RealHum = temp_hum_val[0];
//	Serial.print(temp);
//  Serial.print("   ");
//  Serial.println(RealHum);
  }
}

void MinMax() {
  if ((temp < MinTemp) && (temp>=1)) {
    MinTemp = temp;
    EEPROM.write(2, MinTemp);
    EEPROM.commit();
  }
  else if ((temp > MaxTemp) && (temp<=100)) {
    MaxTemp = temp;
    EEPROM.write(3, MaxTemp);
    EEPROM.commit();
  }
 // else if (RealHum < MinHum && RealHum > 0) {
 //   MinHum = RealHum;
 //   EEPROM.write(4, MinHum);
 //   EEPROM.commit();
 // }
 // else if (RealHum > MaxHum && RealHum < 100) {
 //   MaxHum = RealHum;
 //   EEPROM.write(5, MaxHum);
 //   EEPROM.commit();
 // }
}