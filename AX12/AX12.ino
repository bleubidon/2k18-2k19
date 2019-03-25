#include <DynamixelSerial.h>

#define AX12 4

void setup(){
   Dynamixel.begin(1000000,2);  // Initialize the servo at 1Mbps and Pin Control 2
}

void loop(){
    Dynamixel.move(AX12,0);
    delay(2000);
    Dynamixel.move(AX12,500);
    delay(2000);
    Dynamixel.move(AX12,1000);
    delay(2000);
}
