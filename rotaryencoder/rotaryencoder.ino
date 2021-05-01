#include <LiquidCrystal.h>
LiquidCrystal lcd(6,7,8,9,10,11);  

#define encoder0PinA 3
#define encoder0PinB 4
#define encoder0Btn 5
int encoder0Pos = 0;
unsigned long previousMillis = 0;
const long interval = 150;

void setup() {
  lcd.begin(16, 2);
  pinMode(encoder0PinA, INPUT);
  pinMode(encoder0PinB, INPUT);
  pinMode(encoder0Btn, INPUT);
  attachInterrupt(0, doEncoder, CHANGE);
}
int valRotary,lastValRotary;

void loop() {
unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
 lcd.setCursor(0, 0);   
  lcd.print(valRotary);
  if(valRotary>lastValRotary)
  {
  lcd.print("  CW");
  }
  if(valRotary<lastValRotary)   {

  lcd.print("  CCW");
  }
  lastValRotary = valRotary;
  lcd.print(" ");
}
}
void doEncoder()
{
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))
  {
  encoder0Pos++;
  }
  else
  {
  encoder0Pos--;
  }
  valRotary = encoder0Pos/2.5;
}
