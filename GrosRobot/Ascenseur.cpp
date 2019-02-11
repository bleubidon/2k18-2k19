#include <DynamixelSerial2.h>
#include "Actions.h"
#include <TaskQueue.h>


#define PINCE_GAUCHE 11
#define PINCE_DROITE 6
#define POSITION_OUVERTE_DROITE 50
#define POSITION_FERMEE_DROITE 300
#define POSITION_OUVERTE_GAUCHE 320
#define POSITION_FERMEE_GAUCHE 70

int pinTour = 19;
int pinButee = 28;
int pinsRelais[2] = {24, 26};

int nbTour = 0;
unsigned long last_interrupt_time = 0;
bool bMotor=true;

static int tourPalet = 15;

static void ascenseur_tour()
{
  static int interval = 30;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > interval)
  {
    nbTour++;
  }
  last_interrupt_time = interrupt_time;
}

static void ascenseur_butee()
{
  bMotor = false;
}

void ascenseur_setup()
{
  pinMode(pinTour, INPUT_PULLUP);
  pinMode(pinButee, INPUT_PULLUP);
  Dynamixel.begin(1000000, 2);
  attachInterrupt(digitalPinToInterrupt(pinTour), ascenseur_tour, FALLING);

  for (int pin : pinsRelais)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  ascenseur_initilisation();
}


void ascenseur_monter()
{
  //
}

void ascenseur_initilisation()
{
  Serial << "demarrage initialistation" << endl;
  DEBUG Pince(
  //Ouverture pince
  DEBUG (Serial << "ouverture pince" << endl;)
  Dynamixel.move (PINCE_GAUCHE , POSITION_OUVERTE_GAUCHE ) ; // Position droit : 210 pour ID 11
  delay(20);
  Dynamixel.move (PINCE_DROITE , POSITION_OUVERTE_DROITE ) ; // Position droit : 210 pour ID 11
  DEBUG (Serial << "pince ouverte" << endl;)
  )
  //Descente plateau
  digitalWrite(pinsRelais[0], LOW);
  digitalWrite(pinsRelais[1], HIGH);

  while (digitalRead(pinButee) == HIGH)
    ;
  nbTour = 0;

  DEBUG (Serial << "butée detectée" << endl;)
  
  digitalWrite(pinsRelais[0], HIGH);
  digitalWrite(pinsRelais[1], HIGH);
  DEBUG Pince(
  //Fermeture pince
  DEBUG (Serial << "fermeture pince" << endl;)
  Dynamixel.move (PINCE_GAUCHE , POSITION_FERMEE_GAUCHE) ; // Position droit : 210 pour ID 11
  delay(20);
  Dynamixel.move (PINCE_DROITE , POSITION_FERMEE_DROITE) ; // Position droit : 210 pour ID 11
  DEBUG (Serial << "Pince fermée" << endl;)
  )
  delay(2000);
   
  //Monter plateau
  DEBUG (Serial << "Montee plateau" << endl;)
  digitalWrite(pinsRelais[0], HIGH);
  digitalWrite(pinsRelais[1], LOW);

  while (nbTour < 15)
    ;
    
  digitalWrite(pinsRelais[0], HIGH);
  digitalWrite(pinsRelais[1], HIGH);

  DEBUG (Serial << "Fin de montee (" << nbTour << " tours)" << endl;)
  nbTour = 0;
  
}

void cycle_ascenseur()
{
  Serial << "demarrage descente" << endl;
  
  delay(1000);
  DEBUG Pince(
  //Lacher des palets
  Dynamixel.move (PINCE_GAUCHE , POSITION_OUVERTE_GAUCHE ) ; // Position droit : 210 pour ID 11
  delay(20);
  Dynamixel.move (PINCE_DROITE , POSITION_OUVERTE_DROITE ) ; // Position droit : 210 pour ID 11
  )
  delay(500);

  //Descente plateau
  digitalWrite(pinsRelais[0], LOW);
  digitalWrite(pinsRelais[1], HIGH);
  
  while (digitalRead(pinButee) == HIGH)
    ;
  nbTour = 0;

  Serial << nbTour << endl;
  
  digitalWrite(pinsRelais[0], HIGH);
  digitalWrite(pinsRelais[1], HIGH);
  
  delay(500);
  DEBUG Pince(
  //Fermeture pince
  Dynamixel.move (PINCE_GAUCHE , POSITION_FERMEE_GAUCHE) ; // Position droit : 210 pour ID 11
  delay(20);
  Dynamixel.move (PINCE_DROITE , POSITION_FERMEE_DROITE) ; // Position droit : 210 pour ID 11
  delay(500);
  )
  //Monter plateau
  digitalWrite(pinsRelais[0], HIGH);
  digitalWrite(pinsRelais[1], LOW);
  
  while (nbTour < 15)
    ;
    
  nbTour = 0;

  digitalWrite(pinsRelais[0], HIGH);
  digitalWrite(pinsRelais[1], HIGH);
}
