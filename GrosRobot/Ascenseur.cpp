#include <DynamixelSerial2.h>
#include "Actions.h"
#include <TaskQueue.h>

#define PINCE_GAUCHE 11
#define PINCE_DROITE 6
#define POSITION_OUVERTE_DROITE 50
#define POSITION_FERMEE_DROITE 300
#define POSITION_OUVERTE_GAUCHE 320
#define POSITION_FERMEE_GAUCHE 70

const int pinTour = 19;
const int pinButee = 28;
const int pinsRelais[2] = {24, 26};

int nbTour = 0; // Compteur de tour de l'interrupt
unsigned long last_interrupt_time = 0; // Anti bounce

static void ascenseur_tour()
{
	static const int interval = 30;

	unsigned long interrupt_time = millis();
	if (interrupt_time - last_interrupt_time > interval)
		nbTour++;
	last_interrupt_time = interrupt_time;
}

void setup_ascenseur()
{
	pinMode(pinTour, INPUT_PULLUP);
	pinMode(pinButee, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(pinTour), ascenseur_tour, FALLING);
	Dynamixel.begin(1000000, 2);

	for (int pin : pinsRelais)
	{
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}

	// Cycle pour initialiser la position du plateau
	cycle_ascenseur();
}

void descente_plateau()
{
	digitalWrite(pinsRelais[0], LOW);
	digitalWrite(pinsRelais[1], HIGH);

	while (digitalRead(pinButee) == HIGH)
		;

	digitalWrite(pinsRelais[0], HIGH);
	digitalWrite(pinsRelais[1], HIGH);

	DEBUG(Serial << "Nombre de tours: " << nbTour << endl);
}

void montee_plateau()
{
	// Nombre de tour pour monter de la hauteur d'un palet
	const int nb_tour_palet = 15;

	nbTour = 0;
	digitalWrite(pinsRelais[0], HIGH);
	digitalWrite(pinsRelais[1], LOW);

	while (nbTour < nb_tour_palet)
		;

	digitalWrite(pinsRelais[0], HIGH);
	digitalWrite(pinsRelais[1], HIGH);

	DEBUG(Serial << "Fin de montee (" << nbTour << " tours)" << endl);
}

/*
 * Le plateau est en position haute et il y a un
 * palet dans la zone de recuperation du robot
 * 
 * Cycle de l'ascenseur:
 * 1: Lacher les palets tenus par la pince
 * 2: Descente du plateau jusqu'a la butee
 * 3: Fermer les pinces pour attraper la pile
 * 4: Remonter le plateau
 */
void cycle_ascenseur()
{
	Serial << "Demarrage du cycle" << endl;

	delay(1000);
/*
	// 1- Lacher des palets
	Dynamixel.move (PINCE_GAUCHE , POSITION_OUVERTE_GAUCHE ) ; // Position droit : 210 pour ID 11
	delay(20);
	Dynamixel.move (PINCE_DROITE , POSITION_OUVERTE_DROITE ) ; // Position droit : 210 pour ID 11
	delay(500);
*/

	// 2- Descente plateau
	descente_plateau();
	delay(500);
	
/*
	// 3- Fermeture pince
	Dynamixel.move (PINCE_GAUCHE , POSITION_FERMEE_GAUCHE) ; // Position droit : 210 pour ID 11
	delay(20);
	Dynamixel.move (PINCE_DROITE , POSITION_FERMEE_DROITE) ; // Position droit : 210 pour ID 11
	delay(500);
*/

	// 4- Monter plateau
	montee_plateau();
}