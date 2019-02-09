#include <DynamixelSerial.h>

void setup(){
   Dynamixel.begin(1000000,2);  // Initialize the servo at 1Mbps and Pin Control 2
}

void loop(){
    Dynamixel.move(1,0);
    delay(2000);
    Dynamixel.move(1,500);
    delay(2000);
    Dynamixel.move(1,1000);
    delay(2000);
}
