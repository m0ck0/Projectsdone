#include <Keypad.h>
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
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
byte customKey = 12;
byte hexaKeys[ROWS][COLS] = {
  {'9','7','4','6'},
  {'10','5','2','A'},
  {'8','5','3','1'}};
byte rowPins[ROWS] = {14, 15, 16};
byte colPins[COLS] = {8, 9, 10, 11}; 
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  MIDI.begin();
  Serial.begin(115200);
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
  shuttleCode();
  customKey = customKeypad.getKey();
  if (customKey){midikey();}
}
/*  if (shuttleChange | jogChange){
    Serial.print("valor: ");
    Serial.print(valores);
    Serial.print(" Jog: ");
    Serial.print(jogValue);
}*/
void shaft_moved(){
  if (millis()-last_run>5){
    if (digitalRead(2)==1){
      jogValue++;
      }
    if (digitalRead(2)==0){
      jogValue--; 
    last_run=millis();
  }}}

void shuttleCode() {
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
  if (output == 129) {valores = 115;}}
void midikey(){
  if (customKey==49){MIDI.sendControlChange(1,127,1);}
  if (customKey==50){MIDI.sendControlChange(2,127,1);}
  if (customKey==51){MIDI.sendControlChange(3,127,1);}
  if (customKey==52){MIDI.sendControlChange(4,127,1);}
  if (customKey==53){MIDI.sendControlChange(5,127,1);}
  if (customKey==54){MIDI.sendControlChange(6,127,1);}
  if (customKey==55){MIDI.sendControlChange(7,127,1);}
  if (customKey==56){MIDI.sendControlChange(8,127,1);}
  if (customKey==57){MIDI.sendControlChange(9,127,1);}
  if (customKey==48){MIDI.sendControlChange(10,127,1);}
  if (customKey==65){MIDI.sendControlChange(11,127,1);}
  }
 /* void midikey(){
  if (customKey==49){MIDI.sendNoteOn(60,127,1);Serial.println("Freeze");}
  if (customKey==50){MIDI.sendNoteOn(61,127,1);Serial.println("Index");}
  if (customKey==51){MIDI.sendNoteOn(62,127,1);Serial.println("Mark");}
  if (customKey==52){MIDI.sendNoteOn(63,127,1);Serial.println("Erase");}
  if (customKey==53){MIDI.sendNoteOn(64,127,1);Serial.println("Stop");}
  if (customKey==54){MIDI.sendNoteOn(65,127,1);Serial.println("Rec");}
  if (customKey==55){MIDI.sendNoteOn(66,127,1);Serial.println("Pause");}
  if (customKey==56){MIDI.sendNoteOn(67,127,1);Serial.println("Rew");}
  if (customKey==57){MIDI.sendNoteOn(68,127,1);Serial.println("Play");}
  if (customKey==48){MIDI.sendNoteOn(69,127,1);Serial.println("FFwd");}
  if (customKey==65){MIDI.sendNoteOn(70,127,1);Serial.println("J/S");}
  }*/