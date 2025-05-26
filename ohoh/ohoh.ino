#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
//#include <Wire.h>
#include <EEPROM.h>
#include <OneWire.h>                
#include <DallasTemperature.h>
const char* ssid = "Nichonet";
const char* password = "milanesas";

const unsigned long Minutos = 60 * 1000UL;
byte HumidiOn = 1; 
byte HumidiWait = 1;
byte AirOn = 1; 
byte AirWait = 1;
unsigned long previousMillis = 0;
unsigned long prevMillis = 0;
const long interval = 6000;
const byte Termometro = D4;
const byte Calentador = D5;
const byte Humidi = D6;
const byte Aire = D7;
const byte AireOut = D8;
OneWire oneWire(Termometro);
DallasTemperature sensors(&oneWire);
byte AireW = 0;
String HumidiStatus = "patata";
String CalentadorStatus = "nose";
byte MaxTemp = 0;
byte MinTemp = 100;
byte TargetTemp = 22;
float RealTemp = 0;

ESP8266WebServer server(80);

void handleRoot() {
    File file = LittleFS.open("/index.html", "r");
    if (file) {
        server.streamFile(file, "text/html");
        file.close();
    }
}

void handleCSS() {
    File file = LittleFS.open("/style.css", "r");
    if (file) {
        server.streamFile(file, "text/css");
        file.close();
    }
}
void handleIcon() {
    File file = LittleFS.open("/favicon.ico", "r");
    if (file) {
        server.streamFile(file, "image/ico");
        file.close();
    }
}

void handleJS() {
    File file = LittleFS.open("/script.js", "r");
    if (file) {
        server.streamFile(file, "application/javascript");
        file.close();
    }
}

void handleJson() {
    File file = LittleFS.open("/data.json", "r");
    if (file) {
   String jsonData = file.readString();
    server.send(200, "application/json", jsonData);
    file.close();
    }
}

void handleGetAllData() {
  String jsonData = "{";
  jsonData += "\"RealTemp\": \"" + String(RealTemp) + "°C\",";
  jsonData += "\"MinTemp\": \"" + String(MinTemp) + "°C\",";
  jsonData += "\"MaxTemp\": \"" + String(MaxTemp) + "°C\",";
  jsonData += "\"TargetTemp\": \"" + String(TargetTemp) + "°C\",";
  jsonData += "\"CalentadorStatus\": \"" + CalentadorStatus + "\",";
  jsonData += "\"HumidiStatus\": \"" + HumidiStatus + "\",";
  jsonData += "\"HumidiOn\": \"" + String(HumidiOn) + "\",";
  jsonData += "\"HumidiWait\": \"" + String(HumidiWait) + "\",";
  jsonData += "\"AirOn\": \"" + String(AirOn) + "\",";
  jsonData += "\"AirWait\": \"" + String(AirWait) + "\"";
  jsonData += "}";

  server.send(200, "application/json", jsonData);
}

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  //Wire.begin();
  sensors.begin();
  pinMode (Calentador, OUTPUT);
  pinMode (Humidi, OUTPUT);
  pinMode (Aire, OUTPUT);
  pinMode (AireOut, OUTPUT);
  TargetTemp = EEPROM.read(1);
  MinTemp = EEPROM.read(2);
  MaxTemp = EEPROM.read(3);
  HumidiOn = EEPROM.read(6);
  HumidiWait = EEPROM.read(7);
  AirOn = EEPROM.read(8);
  AirWait = EEPROM.read(9);

  
   WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  
  if (!LittleFS.begin()) {
    Serial.println("Error al montar LittleFS");
    return;
  }  
    server.on("/", handleRoot);
    server.on("/style.css", handleCSS);
    server.on("/script.js", handleJS);
    server.on("/data.json", handleJson);
    server.on("/favicon.ico", handleIcon);
    server.on("/GetAllData", handleGetAllData);


server.on("/TargetTemp-", []() {
    TargetTemp--;
    EEPROM.write(1, TargetTemp);
    EEPROM.commit();
    server.send(204);  
    });
server.on("/TargetTemp+", []() {
    TargetTemp++;
    EEPROM.write(1, TargetTemp);
    EEPROM.commit();
    server.send(204);  
    });

server.on("/NubeOn", []() {
    digitalWrite(Humidi, HIGH);
    digitalWrite(Aire, HIGH);
    AireW = 0;
 	  HumidiStatus = "Humiding";
    Serial.println("Humiding");
    server.send(204);  
    });
server.on("/NubeStop", []() {
    digitalWrite(Humidi, LOW);
    digitalWrite(Aire, LOW);
    AireW = 0;
    HumidiStatus = "Waiting";
    Serial.println("Waiting");
    server.send(204);  
    });


server.on("/HumidiOn-", []() {
    HumidiOn--;
    EEPROM.write(6, HumidiOn);
    EEPROM.commit();
    server.send(204);  
    });
server.on("/HumidiOn+", []() {
    HumidiOn++;
    EEPROM.write(6, HumidiOn);
    EEPROM.commit();
    server.send(204);  
    });

server.on("/HumidiWait-", []() {
    HumidiWait--;
    EEPROM.write(7, HumidiWait);
    EEPROM.commit();
    server.send(204);  
    });
server.on("/HumidiWait+", []() {
    HumidiWait++;
    EEPROM.write(7, HumidiWait);
    EEPROM.commit();
    server.send(204);  
    });

server.on("/AirOn-", []() {
    AirOn--;
    EEPROM.write(8, AirOn);
    EEPROM.commit();
    server.send(204);  
    });
server.on("/AirOn+", []() {
    AirOn++;
    EEPROM.write(8, AirOn);
    EEPROM.commit();
    server.send(204);  
    });

server.on("/AirWait-", []() {
    AirWait--;
    EEPROM.write(9, AirWait);
    EEPROM.commit();
    server.send(204);  
    });
server.on("/AirWait+", []() {
    AirWait++;
    EEPROM.write(9, AirWait);
    EEPROM.commit();
    server.send(204);  
    });

server.on("/ReSet", []() {
    EEPROM.write(1, 6);
    EEPROM.write(2, 6);
    EEPROM.write(3, 6);
    EEPROM.write(6, 6);
    EEPROM.write(7, 6);
    EEPROM.write(8, 6);
    EEPROM.write(9, 6);
    EEPROM.commit();
    server.send(204);  
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
  server.handleClient();


}

void calentador() {
  if (RealTemp<0 && (digitalRead(Calentador))){
  digitalWrite(Calentador, LOW);
  CalentadorStatus = "Off";
  Serial.println("CalentOff (no data)");
    } 
  else if (TargetTemp >= RealTemp && (!digitalRead(Calentador)) && RealTemp>0) {
    digitalWrite(Calentador, HIGH);
    CalentadorStatus = "On";
    Serial.println("CalentOn");
  }
 else if (TargetTemp < RealTemp && (digitalRead(Calentador)) && RealTemp>0) {
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
sensors.requestTemperatures(); 
RealTemp = sensors.getTempCByIndex(0);
	Serial.println(RealTemp);
}
void MinMax() {
  if ((RealTemp < MinTemp) && (RealTemp>=1)) {
    MinTemp = RealTemp;
    EEPROM.write(2, MinTemp);
    EEPROM.commit();
  }
  else if ((RealTemp > MaxTemp) && (RealTemp<=100)) {
    MaxTemp = RealTemp;
    EEPROM.write(3, MaxTemp);
    EEPROM.commit();
  }
}


