#include "Actions.h"
#include <Parser.h>

Parser parser;
Robot paschair;


void setup()
{
	Serial.begin(9600);

	parser.add("mv", deplacement);
	parser.add("ecran", commande_ecran);

	Odometrie::Config odometrie = {
		mode: CODEUSE_GYROSCOPE,
		{
			codeuse: {
				pin: 11,
				rayon: 2.4f,
				nb_pas_max: 800
            }
		}
	};

	Robot::Config config = {
		couleur: GAUCHE,
		dureeMatch: -1, // illimité   // 90000L = 90 secondes

		odometrie,
		moteurs: {
			{4, 9, 6},
			{7, 8, 5}
		},
		pinTirette: 27
	};

	paschair.setup(config);

	define_actions(paschair);

	paschair.waitTirette();
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

void commande_ecran(int argc, char **argv)
{
	// if (argc == 2)
	// 	paschair.ecran.parse(42, argv[1]);
}
