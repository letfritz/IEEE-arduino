int pinLED[9] = {12,11,10,9,8,7,6,5,3};
int numeroLED;
int digitado;

void setup() {
  // put your setup code here, to run once:
  int x;  
  for(x = 0; x <= 8; x++){
    pinMode(pinoLED[x],OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitado = ' ';
  numeroLED = 0;
  while(digitado != 'P'){
    digitadoWrite(pinLED[numeroLED],LOW);
    numeroLED++
    if(numeroLED > 8){
      numeroLED = 0;
    }
    digitalWrite(pinLED[numeroLED],HIGH);
    if(Serial.available()){
      digitado = Serial.read();
    }
    delay(100);
  }
  delay(5000);
}


