int pinLED[9] = {12,11,10,9,8,7,6,5,3};
int numeroLED;
void setup() {
  int x;
  for(x = 0; x <=8; x = x + 1){
    pinMode(pinLED[x],OUTPUT);
  }
}

  void loop() {
  numeroLED = 0;
  while(numeroLED <= 8){
    digitalWrite(pinLED[numeroLED],HIGH);
    delay(200);
    numeroLED = numeroLED + 1;
  }
  numeroLED = 8;
  for(numeroLED >= 0){
    digitalWrite(pinLED[numeroLED],LOW);
    delay(200);
    numeroLED = numeroLED - 1;
  }
}




