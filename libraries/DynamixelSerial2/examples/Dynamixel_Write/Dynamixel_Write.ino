#include <DynamixelSerial2.h>

void setup(){
Dynamixel.begin(1000000,2);  // Inicialize the servo at 1Mbps and Pin Control 2
delay(1000);
}

void loop(){

  Dynamixel.move(6,400);  // Move the Servo radomly from 200 to 800
  delay(100);
  Dynamixel.move(6,800);  // Move the Servo radomly from 200 to 800
  delay(100);
}
