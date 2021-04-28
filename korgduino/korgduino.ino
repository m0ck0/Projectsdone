#include <Arduino_Helpers.h> // Include the Arduino Helpers library
#include <AH/Hardware/ExtendedInputOutput/AnalogMultiplex.hpp>
#include <Control_Surface.h>

HairlessMIDI_Interface midi;

 CD74HC4051 mux[] = { 
  { A0, {2, 3, 4}},
  { A1, {2, 3, 4}}
 };

const AddressMatrix<5, 8> addresses = {{

  {36,  37,  38,  39,  40,  73,  74,  75},
  {44,  45,  46,  47,  48,  41,  42,  43},
  {60,  61,  62,  63,  64,  57,  58,  59},
  {52,  53,  54,  55,  56,  49,  50,  51},
  {68,  69,  70,  71,  72,  65,  66,  67}
  }};
  


 NoteButtonMatrix<5, 8> buttonmatrix = {
  {8, 16, 7, 6, 5},  {mux[0].pin(3), mux[0].pin(4), mux[0].pin(5), mux[0].pin(6), mux[0].pin(7), mux[0].pin(0), mux[0].pin(1), mux[0].pin(2),},
  addresses,    // address matrix
  CHANNEL_1,    // channel and cable number
};

 
CCPotentiometer pots[] = {
    { mux[1].pin(7), { MIDI_CC::Channel_Volume, CHANNEL_1 } },            //vol
    { mux[1].pin(1), { 0x14, CHANNEL_1 } },                               //joyhor
    { mux[1].pin(2), { 0x15, CHANNEL_1 } },                               //pot 1
    { mux[1].pin(3), { 0x16, CHANNEL_1 } },                               //pot 2
    { mux[1].pin(4), { 0x17, CHANNEL_1 } },                               //fader       
    { mux[1].pin(5), { 0x18, CHANNEL_1 } },                               //LDR
    { mux[1].pin(6), { 0x19, CHANNEL_1 } }
};

PBPotentiometer pitchbend = {
  mux[1].pin(0), CHANNEL_1 };
   
void setup() {

  Control_Surface.begin();
    mux[0].begin();                 
    mux[1].begin();                 
    mux[0].pinMode(0, INPUT_PULLUP);
    mux[1].pinMode(0, INPUT_PULLUP);
}      

void loop() {
    Control_Surface.loop();
//    Serial.println(pots[4].getRawValue());
}
