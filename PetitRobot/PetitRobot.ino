#include "PetitRobot.h"

Parser parser;
PetitRobot paschair;

void setup()
{
	Serial.begin(9600);

	parser.add("mv", deplacement);
  parser.add("ecran", ecran);
    
	Config_Robot config = {
		.couleur = GAUCHE,
		.dureeMatch = -1, // 90000L // 90 secondes

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
	parser.loop();
	paschair.loop();
}

// Commands
void deplacement(int argc, char **argv)
{
	if (argc != 3)
		return;

	int val = atoi(argv[2]);

	if (!strcmp(argv[1], "t"))
	{
		Serial << "avance de " << val << endl;
		paschair.setup_avancer(val);
	}
	else if (!strcmp(argv[1], "r"))
    {
		Serial << "rotation de " << val << endl;
		paschair.setup_tourner(val);
    }
}

void ecran(int argc, char** argv)
{
  if (argc == 2)
    paschair.ecran.parse(42, argv[1]);
}
