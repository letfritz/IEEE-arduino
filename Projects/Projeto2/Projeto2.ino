int ledPin=13;
int buttonPin=2;
int buttonEstado=0;

void setup() {

  pinMode(ledPin,OUTPUT);
  pinMode(buttonPin,INPUT);

}

void loop() {

  buttonEstado=digitalRead(buttonPin);

  if(buttonEstado==HIGH){
    digitalWrite(ledPin,HIGH);  
  }else{
    digitalWrite(ledPin,LOW);  
  }
  
}




