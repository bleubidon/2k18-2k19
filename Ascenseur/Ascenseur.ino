void setup() {
  Serial.begin(9600);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(7, LOW);
  digitalWrite(8, LOW);
}

void loop() {
  digitalWrite(8, LOW);
  delay(3000);
  digitalWrite(7,HIGH);
  delay(3000);
  digitalWrite(7, LOW);
  delay(3000);
  digitalWrite(8,HIGH);
  delay(3000);
}
