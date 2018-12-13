void setup() {
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  
}

void loop() {
  digitalWrite(7,LOW);
  delay(3000);
  digitalWrite(7,HIGH);
  delay(2000);
  digitalWrite(8,LOW);
  delay(3000);
  digitalWrite(8,HIGH);
  delay(2000);
}
