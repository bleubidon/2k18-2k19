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

const int pince_gauche_min_value = 140;
const int pince_gauche_max_value = 260;
const int pince_droite_min_value = 90;
const int pince_droite_max_value = 210;

const int opened_pliers_values[2] = {260, 120};  // gauche, droite
const int closed_pliers_values[2] = {190, 210};  // gauche, droite; old: 210, 190


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
    if (gauche >=0) { //On souhaite deplacer la pince gauche
        if (gauche >= pince_gauche_min_value && gauche <= pince_gauche_max_value) { //Intervalle de valeurs autorisees
            Dynamixel.move(pinces[GAUCHE], gauche);
            delay(20);
        }
        else DEBUG(Serial << "Cannot move left plier, value not in authorized range" << endl);
    }

    if (droite >=0) { //On souhaite deplacer la pince droite
        if (droite >= pince_droite_min_value && droite <= pince_droite_max_value) { //Intervalle de valeurs autorisees
            Dynamixel.move(pinces[DROITE], droite);
            delay(500);
        }
        else DEBUG(Serial << "Cannot move right plier, value not in authorized range" << endl);
    }
}

void cycle_ascenseur()
{
	Serial << "Demarrage du cycle" << endl;

	delay(1000);

	// 1- Relachement des palets
	set_pinces(opened_pliers_values[GAUCHE], opened_pliers_values[DROITE]);

	// 2- Descente plateau
	descente_plateau();
	
	// 3- Attrapage des palets
	set_pinces(closed_pliers_values[GAUCHE], closed_pliers_values[DROITE]);

	// 4- Remontée du plateau
	montee_plateau();
}
