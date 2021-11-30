#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include <DHT.h>
#include <EEPROM.h>

DHT dht (D5, DHT22);

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
const byte Calentador = 12;
const byte Humidi = 13;
const byte Aire = 5;
const byte AireW = 4;
String CalentadorStatus = "nose";
String HumidiStatus = "patata";
byte MaxTemp = 0;
byte MinTemp = 100;
byte MaxHum = 0;
byte MinHum = 100;
byte TargetTemp = 22;
byte LastTemp;
int RealHum;
float temp_hum_val[2] = {0};
float RealTemp;

AsyncWebServer server(80);

String processor(const String& var) {
  if (var == "CalentadorStatus") {
   return String(CalentadorStatus);
  }
  else if (var == "HumidiStatus") {
   return String(HumidiStatus);
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
  pinMode (Calentador, OUTPUT);
  pinMode (Humidi, OUTPUT);
  pinMode (Aire, OUTPUT);
  pinMode (AireW, OUTPUT);
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
    digitalWrite(Aire, HIGH);
    HumidiStatus = "On";
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest * request) {
    digitalWrite(Humidi, LOW);
    digitalWrite(Aire, LOW);
    HumidiStatus = "Off";
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
        EEPROM.commit();
        MinTemp = RealTemp;
        MaxTemp = RealTemp;
        MinHum = RealHum;
        MaxHum = RealHum;
        Serial.println("Reset");
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
  server.on("/GetCalentador", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(CalentadorStatus).c_str());
  });  
  server.on("/GetHumidi", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", String(HumidiStatus).c_str());
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
  calentador();
  timerhumi();
}

void calentador() {
  if ((temp_hum_val[1] != 0) && (TargetTemp >= RealTemp) && (LastTemp < RealTemp)) {
    digitalWrite(Calentador, HIGH);
    CalentadorStatus = "On";
  } else if ((temp_hum_val[1] != 0) && ((TargetTemp - LastTemp) >= 1 ) && (LastTemp > RealTemp)) {
    digitalWrite(Calentador, HIGH);
	CalentadorStatus = "On";
	} else {
    digitalWrite(Calentador, LOW);
	CalentadorStatus = "Off";
	}
}

void timerhumi() {
  unsigned long curMillis = millis();
  if ((digitalRead(Aire)) && (!digitalRead(Humidi)) && (curMillis - prevMillis >= (AirOn*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Aire, LOW);
    digitalWrite(AireW, HIGH);
    HumidiStatus = "WaitAir";
    Serial.println("WaitAit");
  }  else if ((digitalRead(AireW)) && (curMillis - prevMillis >= (AirWait*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Humidi, HIGH);
    digitalWrite(Aire, HIGH);
	HumidiStatus = "Humiding";
    Serial.println("Humiding");
  }  else if ((digitalRead(Aire)) && (digitalRead(Humidi)) && (curMillis - prevMillis >= (HumidiOn*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Humidi, LOW);
    digitalWrite(Aire, LOW);
	HumidiStatus = "Waiting";
    Serial.println("Waiting");
  }  else if ((!digitalRead(Aire)) && (!digitalRead(Humidi)) && (curMillis - prevMillis >= (HumidiWait*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Aire, HIGH);
    HumidiStatus = "Airing";
    Serial.println("Airing");
  }else if ((!digitalRead(Aire)) && (!digitalRead(Humidi)) && (!digitalRead(AireW))) {
    HumidiStatus = "Off";
  }
}


void temphum() {
  LastTemp = RealTemp;
  if (!dht.readTempAndHumidity(temp_hum_val)) {
    RealHum = temp_hum_val[0];
    RealTemp = temp_hum_val[1];
  }
}

void MinMax() {
  if (RealTemp < MinTemp && RealTemp != 0) {
    MinTemp = RealTemp;
    EEPROM.write(2, MinTemp);
    EEPROM.commit();
  }
  else if (RealTemp > MaxTemp) {
    MaxTemp = RealTemp;
    EEPROM.write(3, MaxTemp);
    EEPROM.commit();
  }
  else if (RealHum < MinHum && RealTemp != 0) {
    MinHum = RealHum;
    EEPROM.write(4, MinHum);
    EEPROM.commit();
  }
  else if (RealHum > MaxHum) {
    MaxHum = RealHum;
    EEPROM.write(5, MaxHum);
    EEPROM.commit();
  }
}
