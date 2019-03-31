#include "Actions.h"
#include <DynamixelSerial2.h>

// PULLUP donc brancher un cote a la masse (pas au 5V) et l'autre a l'arduino
const int pinBas = 28;
const int pinHaut = 22;
const int pinPalet = 30;
const int pinsRelais[2] = {24, 26}; // 24: IN1, 26: IN2

// AX12
const int pinAX12 = 48;
const int pinces[2] = {11, 6}; // gauche, droite


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

	Dynamixel.begin(1000000, pinAX12);

	montee_plateau();
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

void set_pinces(int gauche, int droite)
{
	Dynamixel.move(pinces[GAUCHE], gauche);
	delay(20);

	Dynamixel.move(pinces[DROITE], droite);
	delay(500);
}

void cycle_ascenseur()
{
	Serial << "Demarrage du cycle" << endl;

	delay(1000);

	// 1- Relachement des palets
	set_pinces(260, 120);

	// 2- Descente plateau
	descente_plateau();
	
	// 3- Attrapage des palets
	set_pinces(210, 190);

	// 4- Remontée du plateau
	montee_plateau();
}
