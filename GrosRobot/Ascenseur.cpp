#include "Actions.h"
#include <DynamixelSerial2.h>

#define PINCE_GAUCHE 11
#define PINCE_DROITE 6
#define POSITION_OUVERTE_DROITE 50
#define POSITION_FERMEE_DROITE 300
#define POSITION_OUVERTE_GAUCHE 320
#define POSITION_FERMEE_GAUCHE 70

 // PULLUP donc brancher l'autre cote du capteur a la masse
const int pinBas = 28;
const int pinHaut = 22;
const int pinPalet = 30;
const int pinsRelais[2] = {24, 26}; // 24: IN1, 26: IN2

void setup_ascenseur()
{
	for (int pin : pinsRelais)
	{
		pinMode(pin, OUTPUT);
		digitalWrite(pin, HIGH);
	}

	pinMode(pinBas, INPUT_PULLUP);
	pinMode(pinHaut, INPUT_PULLUP);
	pinMode(pinPalet, INPUT_PULLUP);

	Dynamixel.begin(1000000, 2);
}

void descente_plateau()
{
	if (digitalRead(pinBas) == LOW)
		return;

	digitalWrite(pinsRelais[0], HIGH);
	digitalWrite(pinsRelais[1], LOW);

	while (digitalRead(pinBas) == HIGH)
		;

	digitalWrite(pinsRelais[0], HIGH);
	digitalWrite(pinsRelais[1], HIGH);
}

void montee_plateau()
{
	if (digitalRead(pinHaut) == LOW)
		return;

	digitalWrite(pinsRelais[0], LOW);
	digitalWrite(pinsRelais[1], HIGH);

	while (digitalRead(pinHaut) == HIGH)
		;

	digitalWrite(pinsRelais[0], HIGH);
	digitalWrite(pinsRelais[1], HIGH);
}

void ouverture_pinces()
{
	// Position droit : 210 pour ID 11
	Dynamixel.move (PINCE_GAUCHE , POSITION_OUVERTE_GAUCHE );
	delay(20);

	// Position droit : 210 pour ID 11
	Dynamixel.move (PINCE_DROITE , POSITION_OUVERTE_DROITE );
	delay(500);
}

void fermeture_pinces()
{
	// Position droit : 210 pour ID 11
	Dynamixel.move (PINCE_GAUCHE , POSITION_FERMEE_GAUCHE);
	delay(20);

	// Position droit : 210 pour ID 11
	Dynamixel.move (PINCE_DROITE , POSITION_FERMEE_DROITE);
	delay(500);
}

void cycle_ascenseur()
{
	Serial << "Demarrage du cycle" << endl;

	delay(1000);

	// 1- Lacher des palets
	ouverture_pinces();

	// 2- Descente plateau
	descente_plateau();
	
	// 3- Fermeture pince
	fermeture_pinces();

	// 4- Monter plateau
	montee_plateau();
}
