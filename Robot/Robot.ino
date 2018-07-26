#include <Timer.h>
#include <I2CParser.h>

#include "Actions.h"

// NOTE Parser: mettre l'option fin de ligne dans la console Arduino pour pouvoir envoyer des commandes

I2CParser ecran;
Parser parser;

Robot paschair;
Timer timer(500);

void setup()
{
	Serial.begin(9600);

	parser.add("mv", deplacement);
	parser.add("pid", set_pid);
	parser.add("consigne", set_consigne);
	//parser.add("ecran", commande_ecran);

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
		P: 1.0,
		I: 1.0,
		D: 1.0,
		
		couleur: GAUCHE,
		dureeMatch: 90000L, // = 90 secondes

		odometrie,
		moteurs: {
			{4, 9, 6},
			{7, 8, 5}
		},
		pinTirette: 27
	};

	paschair.setup(config);

	ecran.setup();
	requestColor();

	setup_actions(paschair);

	paschair.waitTirette();
}

void requestColor()
{
	ecran.parse(42, "couleur 63488 1806"); // Commande de choix de couleur
	Serial << "Enter a color" << endl;

	char* answer;
	do {
		delay(1000);
		answer = ecran.requestFrom(42, 1);
	}
	while (*answer == '\0');

	Serial << "Color is " << answer << endl;
}


void loop()
{
	parser.loop();
	paschair.loop();

	loop_actions();

	//loop_ecran();
}

void loop_ecran()
{
	if (timer.on())
	{
		static char command[15];

		//sprintf(command, "pos %d %d", (int)paschair.position.getAlpha(), (int)paschair.position.codeuse.getDistance());

		ecran.parse(42, command);
	}
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

void set_pid(int argc, char **argv)
{
	if (argc != 4)
		return;
		
	paschair.set_coefs_PID(atof(argv[1]), atof(argv[2]), atof(argv[3]));
}

void set_consigne(int argc, char **argv)
{
	if (argc != 4)
		return;
	
	paschair.set_consigne(atof(argv[1]), atof(argv[2]));
}