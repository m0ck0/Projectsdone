//#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <DHT.h>
DHT dht(2, DHT22);

const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
unsigned long previousMillis = 0;
const long interval = 2000;
const byte Calentador = 12;
const byte Humidi = 13;
byte MaxTemp = 0;
byte MinTemp = 100;
byte MaxHum = 0;
byte MinHum = 100;
byte TargetTemp;
int RealHum;
float temp_hum_val[2] = {0};
float RealTemp;

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
    pinMode (Calentador, OUTPUT);
    pinMode (Humidi, OUTPUT);
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
      if (!dht.readTempAndHumidity(temp_hum_val)) {
        RealHum = temp_hum_val[0];
        RealTemp = temp_hum_val[1];
      }
    }
  }
