int humidi =  13;
unsigned long previousMillis = 0;
const unsigned long HumidiOn = 2 * 1000;
const unsigned long HumidiWait = 60 * 60 * 1000UL;

void setup() {
  pinMode(humidi, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if ((digitalRead(humidi)) && (currentMillis - previousMillis >= HumidiOn)) {
    previousMillis = currentMillis;
    digitalWrite(humidi, LOW);
  }  else if ((!digitalRead(humidi)) && (currentMillis - previousMillis >= HumidiWait)) {
    previousMillis = currentMillis;
    digitalWrite(humidi, HIGH);
  }
}
