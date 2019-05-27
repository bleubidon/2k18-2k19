#include "Actions.h"
#include <DynamixelSerial2.h>

// Capteurs butee: PULLUP donc brancher un cote a la masse (pas au 5V) et l'autre a l'arduino
const int pinBas = 28;
const int pinHaut = 22;
const int pinPalet = 30;
// Relais
const int pinsRelais[2] = {24, 26}; // 24: IN1, 26: IN2

// AX12
const int pinAX12 = 48;
const int pinces[2] = {11, 6}; // gauche, droite

const int min_pliers_values[2] = {170,  0}; // 170 ; 90
const int max_pliers_values[2] = {500, 210}; // 300 ; 210

const int init_pliers_values[2] = {450, 0};

const int opened_pliers_values[2] = {290, 150}; // gauche, droite ; avant : 290 ; 180
const int closed_pliers_values[2] = {240, 185}; // gauche, droite


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
  set_pinces(init_pliers_values[0], init_pliers_values[1]);
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
	const int cmds[2] = {gauche, droite};
	const char *names[2] = {"left", "right"};

	for (int i = 0; i < 2; i++)
	{
		if (cmds[i] < 0) //On ne souhaite pas bouger cette pince
			continue;

		if (min_pliers_values[i] <= cmds[i] && cmds[i] <= max_pliers_values[i])
		{
			Dynamixel.move(pinces[i], cmds[i]);
			delay(20);
		}
		else
			DEBUG(Serial << "Cannot move " << names[i] << " plier: " <<
				"value not in authorized range" << endl);
	}
}

void cycle_ascenseur()
{
	Serial << "Demarrage du cycle" << endl;

	delay(1000);

	// 1- Relachement des palets
	set_pinces(opened_pliers_values[GAUCHE], opened_pliers_values[DROITE]);
  delay(500);
  
	// 2- Descente plateau
	descente_plateau();
	
	// 3- Attrapage des palets
	set_pinces(closed_pliers_values[GAUCHE], closed_pliers_values[DROITE]);
  delay(500);
  
	// 4- Remontée du plateau
	montee_plateau();
}
