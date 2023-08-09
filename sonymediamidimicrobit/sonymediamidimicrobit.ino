#include <Keypad.h>
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();
#define ROWS 3
#define COLS 4
#define SHUTTLE1  2
#define SHUTTLE2 13
#define SHUTTLE3 15
#define SHUTTLE4 16
#define ROW 28
#define LED 11
#define COL 10
#define CLK 0
#define DT 3
byte valores = 0;
int counter = 68 ;
int prevcounter;
byte currentStateCLK;
byte lastStateCLK;
byte shuttleValue = 0; 
byte prevShuttleValue = 0;
byte output = 0b10000000;
byte customKey;
byte ledState = LOW;
byte lastButtonState = LOW;
byte buttonState ;
byte rowPins[ROWS] = {1, 8, 9};
byte colPins[COLS] = {4, 5, 6, 7}; 
byte hexaKeys[ROWS][COLS] = {
  {'9','7','4','6'},
  {'10','5','2','A'},
  {'8','5','3','1'}};
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  MIDI.begin();
  Serial.begin(115200);
  pinMode(SHUTTLE1, INPUT_PULLUP);
  pinMode(SHUTTLE2, INPUT_PULLUP);
  pinMode(SHUTTLE3, INPUT_PULLUP);
  pinMode(SHUTTLE4, INPUT_PULLUP);
	pinMode(CLK,INPUT);
	pinMode(DT,INPUT);
	lastStateCLK = digitalRead(CLK);
  pinMode(COL, OUTPUT);
  digitalWrite(COL, LOW);
  pinMode(ROW, OUTPUT);
}

void loop(){
  buttonState = LOW;
  byte shuttleValue = output;
  bool shuttleChange = (shuttleValue != prevShuttleValue);
  prevShuttleValue = shuttleValue;
  bitWrite(output, 0 , !digitalRead(SHUTTLE4));
  bitWrite(output, 1 , !digitalRead(SHUTTLE3));
  bitWrite(output, 2 , !digitalRead(SHUTTLE2));
  bitWrite(output, 3 , !digitalRead(SHUTTLE1));
  customKey = customKeypad.getKey();
  if (customKey){midikey();}
  if (shuttleChange){shuttleCode();}
  if (buttonState != lastButtonState) {
    lastButtonState = buttonState;
    if (buttonState == LOW) {
      ledState = (ledState == HIGH) ? LOW: HIGH;
      digitalWrite(ROW, ledState);
      digitalWrite(LED, ledState);
    }}

  bool jogChange = (counter != prevcounter);
  prevcounter = counter;
	if (counter > 127){counter = 127;}
	if (counter < 0){counter = 0;}
  if (jogChange){MIDI.sendControlChange(111,(counter),1);} 
  jog();
}

void jog() {
    	currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
		if (digitalRead(DT) != currentStateCLK) {
			counter --;
		} else {
			counter ++;
		}
	}
	lastStateCLK = currentStateCLK;}
void shuttleCode(){
  if (output == 137) {MIDI.sendControlChange(100,0,1); valores = 101;}
  if (output == 139) {MIDI.sendControlChange(100,9,1); valores = 102;}
  if (output == 143) {MIDI.sendControlChange(100,18,1); valores = 103;}
  if (output == 141) {MIDI.sendControlChange(100,27,1); valores = 104;}
  if (output == 140) {MIDI.sendControlChange(100,36,1); valores = 105;}
  if (output == 142) {MIDI.sendControlChange(100,45,1); valores = 106;}
  if (output == 138) {MIDI.sendControlChange(100,54,1); valores = 107;}
  if (output == 136) {MIDI.sendControlChange(100,63,1); valores = 108;}
  if (output == 128) {MIDI.sendControlChange(100,63,1); valores = 108;}
  if (output == 130) {MIDI.sendControlChange(100,72,1); valores = 109;}
  if (output == 134) {MIDI.sendControlChange(100,81,1); valores = 110;}
  if (output == 132) {MIDI.sendControlChange(100,90,1); valores = 111;}
  if (output == 133) {MIDI.sendControlChange(100,99,1); valores = 112;}
  if (output == 135) {MIDI.sendControlChange(100,108,1); valores = 113;}
  if (output == 131) {MIDI.sendControlChange(100,117,1); valores = 114;}
  if (output == 129) {MIDI.sendControlChange(100,127,1); valores = 115;}}
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
  if (customKey==65){MIDI.sendControlChange(11,127,1);buttonState = HIGH;}
  }
 