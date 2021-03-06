#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <DHT.h>
LiquidCrystal lcd(6, 7, 11, 8, 9, 10);
DHT dht(2, DHT22);

const unsigned long Minutos = 60 * 1000UL;
unsigned long previousMillis = 0;
unsigned long prevMillis = 0;
unsigned long HumidiOn; 
unsigned long HumidiWait;
const long interval = 2000;
const byte data = 3;
const byte clk = 4;
const byte boton = 5;
const byte Calentador = 12;
const byte Humidi = 13;
const byte reset = 14;
byte MaxTemp;
byte MinTemp;
byte MaxHum;
byte MinHum;
byte Display = 0;
byte TargetTemp;
byte IntTemp;
byte LastTemp;
byte RealTemp;
int RealHum;
int State;
int LastState;
float temp_hum_val[2] = {0};

byte termometru[8] = {  B00100,  B01010,  B01010,  B01110,  B01110,  B11111,  B11111,  B01110};
byte picagota[8] =   {  B00100,  B00100,  B01010,  B01010,  B10001,  B10001,  B10001,  B01110};
byte lamparica [8] = {  B01110,  B10001,  B10001,  B10001,  B01010,  B01110,  B01110,  B00100};
byte lamparicon[8] = {  B01110,  B11111,  B11111,  B11111,  B01110,  B01110,  B01010,  B00100};
byte sadface[8] =    {  B00000,  B11011,  B11011,  B00010,  B00001,  B01110,  B10001,  B00000};
byte downarrow[8] =  {  B00000,  B00000,  B00000,  B01110,  B01110,  B11111,  B01110,  B00100};
byte uparrow[8] =    {  B00100,  B01110,  B11111,  B01110,  B01110,  B00000,  B00000,  B00000};
byte gotallena[8] =  {  B00100,  B00100,  B01110,  B01110,  B11111,  B11111,  B11111,  B01110};
byte leftarrow[8] =	 {  B00000,  B00000,  B00100,  B01000,  B11111,  B01000,  B00100,  B00000};

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
  lcd.createChar(9, leftarrow);
  pinMode (Calentador, OUTPUT);
  pinMode (Humidi, OUTPUT);
  pinMode (clk, INPUT);
  pinMode (data, INPUT);
  pinMode (boton, INPUT);
  pinMode (reset, INPUT);
  digitalWrite (reset, HIGH);
  //  digitalWrite (boton, HIGH);
  LastState = digitalRead(clk);
  TargetTemp = EEPROM.read(1);
  MinTemp = EEPROM.read(2);
  MaxTemp = EEPROM.read(3);
  MinHum = EEPROM.read(4);
  MaxHum = EEPROM.read(5);
  HumidiOn = EEPROM.read(6);
  HumidiWait = EEPROM.read(7);
}

void loop() {
  if (digitalRead(reset) == LOW) {
    delay(400);
    EEPROM.update(2, RealTemp);
    EEPROM.update(3, RealTemp);
    EEPROM.update(4, RealHum);
    EEPROM.update(5, RealHum);
  }
  if (digitalRead(boton) == LOW) {
    delay(400);
    Display ++;
    if (Display > 5) {
      lcd.clear();
      Display = 1;
    }
    switch (Display) {
      case 1: {
          mainpage();
        }
      case 2: {
          mainpage();
          encoderTemp();
        }
      case 3: {
          mainpage();
          encoderHumOn();
        }
      case 4: {
          mainpage();
          encoderHumOff();
        }
      case 5: {
          logpage();
        }
    }
  }
  timerhumi();
  mainpage();
  calentador();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    temphum();
    MinMax();
  }
}

void encoderTemp() {
  if (TargetTemp < 6 ) TargetTemp = 6;
  if (TargetTemp > 40) TargetTemp = 40;
  State = digitalRead(clk);
  if (State != LastState) {
    if (digitalRead(data) != State) {
      TargetTemp ++;
    } else {
      TargetTemp --;
    }
  }
  LastState = State;
  EEPROM.update (1, TargetTemp);
  lcd.setCursor(15, 0);
  lcd.write(9);
  delay(100);
}

void encoderHumOn() {
  State = digitalRead(clk);
  if (State != LastState) {
    if (digitalRead(data) != State) {
      HumidiOn ++;
    } else {
      HumidiOn --;
    }
  }
  LastState = State;
  EEPROM.update (6, HumidiOn);
  lcd.setCursor(10, 1);
  lcd.write(9);
  delay(100);
}

void encoderHumOff() {
  State = digitalRead(clk);
  if (State != LastState) {
    if (digitalRead(data) != State) {
      HumidiWait ++;
    } else {
      HumidiWait --;
    }
  }
  LastState = State;
  EEPROM.update (7, HumidiWait);
  lcd.setCursor(15, 1);
  lcd.write(9);
  delay(100);
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
  if ((digitalRead(Humidi)) && (curMillis - prevMillis >= (HumidiOn*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Humidi, LOW);
  }  else if ((!digitalRead(Humidi)) && (curMillis - prevMillis >= (HumidiWait*Minutos))) {
    prevMillis = curMillis;
    digitalWrite(Humidi, HIGH);
  }
}

void temphum() {
  if (RealTemp != IntTemp) {
    LastTemp = IntTemp;
    IntTemp = RealTemp;
  }
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
    EEPROM.update(2, MinTemp);
  }
  if (RealTemp > MaxTemp) {
    MaxTemp = RealTemp;
    EEPROM.update(3, MaxTemp);
  }
  if (RealHum < MinHum && RealTemp != 0) {
    MinHum = RealHum;
    EEPROM.update(4, MinHum);
  }
  if (RealHum > MaxHum) {
    MaxHum = RealHum;
    EEPROM.update(5, MaxHum);
  }
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
  Display = 1;
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
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(1);
  if (RealTemp < 10) {
    lcd.print(" ");
  }
  lcd.print(temp_hum_val[1], 1);
  lcd.setCursor(5, 0);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(9, 0);
  if (TargetTemp < 10) {
    lcd.print(" ");
  }
  lcd.print(TargetTemp);
  lcd.print(".0");
  lcd.setCursor(13, 0);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(2, 1);
  if (RealHum < 10) {
    lcd.print(" ");
  }
  lcd.print(RealHum);
  lcd.setCursor(4, 1);
  lcd.print("%");
  lcd.setCursor(7, 1);
  lcd.write(8);
  if (HumidiOn < 10) {
    lcd.print(" ");
  }
  lcd.print(HumidiOn);
  lcd.setCursor(12, 1);
  lcd.write(2);
  if (HumidiWait < 10) {
    lcd.print(" ");
  }
  lcd.print(HumidiWait);
  icons();
}

void icons() {
  if (digitalRead(Calentador)) {
    lcd.setCursor(8, 0);
    lcd.write(4);
  } else {
    lcd.setCursor(8, 0);
    lcd.write(3);
  }

  if (digitalRead(Humidi)) {
    lcd.setCursor(0, 1);
    lcd.write(8);
  } else {
    lcd.setCursor(0, 1);
    lcd.write(2);
  }
}
