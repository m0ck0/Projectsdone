#include <Keypad.h>
const byte ROWS = 3;
const byte COLS = 4;
const int SHUTTLE1 = 4;
const int SHUTTLE2 = 5;
const int SHUTTLE3 = 6;
const int SHUTTLE4 = 7;
byte shuttleValue = 0; 
byte prevShuttleValue = 0;
byte output = 0b10000000;
int valores = 0;
int jogValue = 0;
int prevJogValue = 0;
unsigned long last_run=0;

char hexaKeys[ROWS][COLS] = {
  {'9','7','4','6'},
  {'A','5','2','B'},
  {'8','5','3','1'}
};
byte rowPins[ROWS] = {14, 15, 16};
byte colPins[COLS] = {8, 9, 10, 11}; 
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  pinMode(SHUTTLE1, INPUT_PULLUP);
  pinMode(SHUTTLE2, INPUT_PULLUP);
  pinMode(SHUTTLE3, INPUT_PULLUP);
  pinMode(SHUTTLE4, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), shaft_moved, FALLING);
  pinMode(2,INPUT);
}

void loop(){
  byte shuttleValue = output;
  bool shuttleChange = (shuttleValue != prevShuttleValue);
  prevShuttleValue = shuttleValue;
  bool jogChange = (jogValue != prevJogValue);
  prevJogValue = jogValue;

  bitWrite(output, 0 , !digitalRead(SHUTTLE4));
  bitWrite(output, 1 , !digitalRead(SHUTTLE3));
  bitWrite(output, 2 , !digitalRead(SHUTTLE2));
  bitWrite(output, 3 , !digitalRead(SHUTTLE1));
  convert();

  char customKey = customKeypad.getKey();
  if (shuttleChange | jogChange | customKey){
    Serial.print("valor: ");
    Serial.print(valores);
    Serial.print(" Jog: ");
    Serial.print(jogValue);
    Serial.print(" Key: ");
    Serial.println(customKey);
}}

void shaft_moved(){
  if (millis()-last_run>5){
    if (digitalRead(2)==1){
      jogValue++;
      }
    if (digitalRead(2)==0){
      jogValue--; 
    last_run=millis();
  }
}}

void convert() {
  if (output == 137) {valores = 101;}
  if (output == 139) {valores = 102;}
  if (output == 143) {valores = 103;}
  if (output == 141) {valores = 104;}
  if (output == 140) {valores = 105;}
  if (output == 142) {valores = 106;}
  if (output == 138) {valores = 107;}
  if (output == 136) {valores = 108;}
  if (output == 128) {valores = 108;}
  if (output == 130) {valores = 109;}
  if (output == 134) {valores = 110;}
  if (output == 132) {valores = 111;}
  if (output == 133) {valores = 112;}
  if (output == 135) {valores = 113;}
  if (output == 131) {valores = 114;}
  if (output == 129) {valores = 115;}
}
