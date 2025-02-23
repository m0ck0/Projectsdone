byte LED1 = 10; 
byte LED2 = 11; 
byte hallPin = A3; 
int LEDs = LOW;
int brightness = 0;  // how bright the LED is
int fadeAmount = 5;  // how many points to fade the LED by
const int offset = 532;
float value;

unsigned long previousMil = 0;
unsigned long previousMillis = 0;
const long interval = 20;
const long inter = 200;

void setup() {
  Serial.begin (9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
}

void loop() {
LED();
HALL();
breathe();
}

void LED(){
  if (value<3 && value>-3){
      LEDs = LOW;
    digitalWrite(LED1, LEDs);
    analogWrite(LED2, brightness);
    }
  if (value>3 or value<-3){
      LEDs = HIGH;
    digitalWrite(LED1, LEDs);
    digitalWrite(LED2, !LEDs);
    }
}


void HALL(){ 
unsigned long currentMil = millis();
  if (currentMil - previousMil >= inter) {
    previousMil = currentMil;
  value = (analogRead(hallPin) - offset);
  Serial.print("Value: ");
  Serial.print(value, 1);
  }
}

void breathe() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
//   analogWrite(LED1, brightness);
    brightness = brightness + fadeAmount;
      if (brightness <= 0 || brightness >= 255) {
        fadeAmount = -fadeAmount;
     }
  }
}