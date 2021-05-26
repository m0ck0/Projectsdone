const int boton = 2;
static int Display = 0;
int BotonState = 0;
int BotonPrevState = 0;

void setup() {  
	lcd.begin(16, 2);
	pinMode(boton,INPUT);
	mainpage();
}

void loop() {  
 BotonState = digitalRead(boton);
  if (BotonState != BotonPrevState) {
    if (BotonState == LOW) {
          Display = Display + 1;
          delay(10);
    }
  }
  if (hits==0){
    mainpage();
  }
	if(hits==1){
	humedadA();
  }else
	if(hits==2){
	humedadB();
  }else
	if (hits==3){
    logpage();
  }
	if (hits>=3){
     hits = 0;
  }
delay(200);
}