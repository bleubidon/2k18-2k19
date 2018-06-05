#include "PetitRobot.h"

PetitRobot paschair;

void setup()
{
	Serial.begin(9600);
	
	Config_Robot config = {
		.couleur = GAUCHE,
		.dureeMatch = 90000L, // 90 secondes

		.pinTirette = 27,
		.pinMoteurs = {
			{4, 9, 6},
			{7, 8, 5}
		}
	};

    paschair.setup(config);
}

void loop()
{
	paschair.loop();
}
