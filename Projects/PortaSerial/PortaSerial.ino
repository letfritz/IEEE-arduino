char X;
char Y;
char Z;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  X = 'João';
  Y = 'Zezinho';
  Z = '+';
  
  Serial.print("valorX=");
  Serial.print(X);

  Serial.print(" valorY=");
  Serial.print(Y);

  Serial.print(" valorZ=");
  Serial.println(Z);
}

void loop() {
  // put your main code here, to run repeatedly:

}
