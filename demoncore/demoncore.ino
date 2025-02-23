#include <Breathe.h>
Breathe Breathe;
byte LED1 = 10; 
byte LED2 = 11; 
byte hallPin = A3; 
const int offset = 532;
float value;

unsigned long previousMil = 0;
const long inter = 200;

void setup() {
  Serial.begin (9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  }

void loop() {
LED();
HALL();
  }

void LED(){
  if (value<3 && value>-3){
    Breathe.set(LED1, LOW), 10;
    Breathe.set(LED2, HIGH, 0.7, 10);
    }
  if (value>3 or value<-3){
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    }
  }

void HALL(){ 
  unsigned long currentMil = millis();
    if (currentMil - previousMil >= inter) {
      previousMil = currentMil;
      value = (analogRead(hallPin) - offset);
       Serial.print("Value: ");
       Serial.println(value, 1);
      }
  }

