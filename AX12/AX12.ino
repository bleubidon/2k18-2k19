
#include <DynamixelSerial.h> 
#define PINCE_GAUCHE 11
#define PINCE_DROITE 7
#define POSITION_OUVERTE_DROITE 150
#define POSITION_FERMEE_DROITE 350
#define POSITION_OUVERTE_GAUCHE 250
#define POSITION_FERMEE_GAUCHE 185

void setup() {
  // put your setup code here, to run once:
  Dynamixel.begin(1000000, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Dynamixel.move (9 , POSITION_OUVERTE_GAUCHE) ; // Position droit : 210 pour ID 11
  
  
  //Dynamixel.move (PINCE_GAUCHE , POSITION_OUVERTE_GAUCHE ) ; // Position droit : 210 pour ID 11
  //delay(20);
  Dynamixel.move (PINCE_DROITE , POSITION_OUVERTE_DROITE ) ; // Position droit : 210 pour ID 11
  delay(5000);
 // Dynamixel.move (PINCE_GAUCHE , POSITION_FERMEE_GAUCHE) ; // Position droit : 210 pour ID 11
 // delay(20);
  Dynamixel.move (PINCE_DROITE , POSITION_FERMEE_DROITE) ; // Position droit : 210 pour ID 11
  delay(5000);
  
//  Dynamixel.move (PINCE_GAUCHE , POSITION_OUVERTE_GAUCHE ) ; // Position droit : 210 pour ID 11
 // delay(20);
 // Dynamixel.move (PINCE_DROITE , POSITION_OUVERTE_DROITE ) ; // Position droit : 210 pour ID 11
 // delay(4000);
  

  
/*
  Dynamixel.move (PINCE_GAUCHE , 205) ; // Position droit : 210 pour ID 11
  delay(20);
  Dynamixel.move (PINCE_DROITE , 205) ; // Position droit : 210 pour ID 11
  delay(1000);
  */
  
}

