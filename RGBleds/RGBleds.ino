const int Red = 10;
const int Green = 9;
const int Blue = 11;
const int fadePotIn =A3;
const int redIn = A2; 
const int greenIn = A1; 
const int blueIn = A0; 
const int Redb = 5; 
const int Blueb = 6; 
const int Greenb = 3;
const int botonA = 2;
const int botonB = 4;
//Set Initial Values
int RedVal = 0;
int GreenVal = 0;
int BlueVal = 0;
// Delay Time
int fade = 10;  

// Colour Value 1
int RedVal1 = 250;
int GreenVal1 = 0;
int BlueVal1 = 85;
// Colour Value 2
int RedVal2 = 250;
int GreenVal2 = 5;
int BlueVal2 = 10;

//Set initial mode (Colour Value Mode) to Colour Value 1
int mode = 1;
int redValb; 
int greenValb; 
int blueValb; 
int fadePot;

void setup()
{
//   Serial.begin(9600);
   pinMode(botonA, INPUT_PULLUP); 
   pinMode(botonB, INPUT_PULLUP); 
   pinMode(Red, OUTPUT);
   pinMode(Green, OUTPUT);
   pinMode(Blue, OUTPUT);
   pinMode(Redb, OUTPUT);
   pinMode(Greenb, OUTPUT);
   pinMode(Blueb, OUTPUT);
   analogWrite(Red, RedVal);
   analogWrite(Green, GreenVal);
   analogWrite(Blue, BlueVal);
  }

void loop()       

{
  while(mode == 1){
    if(RedVal < RedVal1){              // If RedVal is less than desired RedVal1
      RedVal ++;                       // increment RedVal
    } else if(RedVal > RedVal1){       // If RedVal is more than desired RedVal1
      RedVal --;                       // decrement RedVal
    } else if(RedVal == RedVal1){      // If RedVal is equal to desired RedVal1
    }
    if(BlueVal < BlueVal1){
      BlueVal ++;
    } else if(BlueVal > BlueVal1){
      BlueVal --;
    } else if(BlueVal == BlueVal1){
    }
    if(GreenVal < GreenVal1){
      GreenVal ++;
    } else if (GreenVal > GreenVal1){
      GreenVal --;
    } else if (GreenVal == GreenVal1){
    }
    analogWrite(Red, RedVal);
    analogWrite(Green, GreenVal);
    analogWrite(Blue, BlueVal);
    delay(fade);                       // Implement a bit of delay to slow the change of colour down a bit
    checkpots();
    if(RedVal == RedVal1 && GreenVal == GreenVal1 && BlueVal == BlueVal1){ // If RedVal & GreenVal & BlueVal are all at the desired values
      mode = 2;                        // Change the mode to the next colour. Exiting this while loop and into the next one
    }
  }

  while(mode == 2){
    if(RedVal < RedVal2){
      RedVal ++;                     
    } else if(RedVal > RedVal2){
      RedVal --;                     
    } else if(RedVal == RedVal2){
    }
    if(BlueVal < BlueVal2){
      BlueVal ++;
    } else if(BlueVal > BlueVal2){
      BlueVal --;
    } else if(BlueVal == BlueVal2){
    }
    if(GreenVal < GreenVal2){
      GreenVal ++;
    } else if (GreenVal > GreenVal2){
      GreenVal --;
    } else if (GreenVal == GreenVal2){
    }
    analogWrite(Red, RedVal);
    analogWrite(Green, GreenVal);
    analogWrite(Blue, BlueVal);
    delay(fade);
    checkpots();
    if(RedVal == RedVal2 && GreenVal == GreenVal2 && BlueVal == BlueVal2){
      mode = 1;
    }
  }
}
void checkpots(){
fadePot = analogRead(fadePotIn); 
redValb = analogRead(redIn); 
greenValb = analogRead(greenIn); 
blueValb = analogRead(blueIn); 
fadePot = map(fadePot, 0, 1023, 0, 600); 
redValb = map(redValb, 0, 1023, 0, 255); 
greenValb = map(greenValb, 0, 1023, 0, 255); 
blueValb = map(blueValb, 0, 1023, 0, 255); 
 if (digitalRead(botonA) == LOW) {
    RedVal1 = redValb;
    GreenVal1 = greenValb;
    BlueVal1 = blueValb;
}
 if (digitalRead(botonB) == LOW) {
    RedVal2 = redValb;
    GreenVal2 = greenValb;
    BlueVal2 = blueValb;
}
  fade = fadePot;
  analogWrite(Redb, redValb); 
  analogWrite(Greenb, greenValb); 
  analogWrite(Blueb, blueValb); 
/*  Serial.print("R ");
  Serial.print(redValb);
  Serial.print("  G ");
  Serial.print(greenValb); 
  Serial.print("  B ");
  Serial.print(blueValb); 
  Serial.print("  |  R1 ");
  Serial.print(RedVal1); 
  Serial.print("  G1 ");
  Serial.print(GreenVal1); 
  Serial.print("  B1 ");
  Serial.print(BlueVal1); 
  Serial.print("  |   R2 ");
  Serial.print(RedVal2); 
  Serial.print("  G2 ");
  Serial.print(GreenVal2); 
  Serial.print("  B2 ");
  Serial.print(BlueVal2); 
  Serial.print("  |  Ro ");
  Serial.print(RedVal); 
  Serial.print("  Go ");
  Serial.print(GreenVal); 
  Serial.print("  Bo ");
  Serial.print(BlueVal); 
  Serial.print(" |   fade ");
  Serial.print(fadePot); 
  Serial.print(" | ");
  Serial.print(fade); 
  Serial.println();
*/
}
