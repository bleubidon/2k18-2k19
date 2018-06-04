#include "PetitRobot.h"

void setup()
{
	Serial.begin(9600);

	Config_Robot config = {
		.couleur = GAUCHE,
		.dureeMatch = 900L,

		.pinTirette = 27,
		.pinMoteurs = {
			{7, 8, 5},
			{4, 9, 6}
		}
	};

	PetitRobot paschair(config);
}

void loop()
{}