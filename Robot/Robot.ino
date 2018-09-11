#include <Timer.h>
#include <I2CParser.h>

#include "Actions.h"

#define ADRESSE_ECRAN 42

// NOTE Parser: mettre l'option fin de ligne dans la console Arduino pour pouvoir envoyer des commandes

I2CParser ecran;
Parser parser;

Timer timer(500);

int control;

void setup()
{
	control = false;

	Serial.begin(9600);

	Serial << "Setup parser..." << endl;
	parser.add("mv", deplacement);
	parser.add("stop", deplacement);
	parser.add("pid", set_pid);
	parser.add("consigne", set_consigne);
	//parser.add("ecran", commande_ecran);

	Serial << "Setup Robot..." << endl;
	Robot.setup({
		P : 1.0,
		I : 1.0,
		D : 1.0,

		couleur : GAUCHE,
		dureeMatch : 90000L, // = 90 secondes

		odometrie : {
			mode : CODEUSE_GYROSCOPE,
			{
				codeuse : {
					pin : 11,
					radius : 2.4f,
					step_per_round : 800
				}
			}
		},
		moteurs : {
			{4, 9, 6, GAUCHE, wheel_radius: 3.0f},
			{7, 8, 5, DROITE, wheel_radius: 3.0f}
		},
		pinTirette : 27
	});

	Serial << "Setup ecran..." << endl;
	ecran.setup();
	//requestColor();

	Serial << "Setup task queues..." << endl;
	setup_actions();
	

	Serial << "Setup done !" << endl;
	Robot.waitTirette();
}

void requestColor()
{
	ecran.write(ADRESSE_ECRAN, "couleur 63488 1806"); // Commande de choix de couleur
	Serial << "Enter a color" << endl;

	char *answer;
	do {
		delay(1000);
		answer = ecran.read(ADRESSE_ECRAN, 1);
	}
	while (*answer == '\0');

	Serial << "Color is " << answer << endl;
}

void loop()
{
	parser.loop();
	Robot.loop();

	loop_actions(control);

	loop_ecran();
}

void loop_ecran()
{
	if (timer.on())
	{
		static char command[15];
		sprintf(command, "pos %d %d", (int)Robot.position.getX(), (int)Robot.position.getY());

		ecran.write(ADRESSE_ECRAN, command);
	}
}

// Commands
void deplacement(int argc, char **argv)
{
	control = (strcmp(argv[0], "mv") == 0);
	Serial << "control to: " << control << endl;

	if (argc != 3)
		return;

	int val = atoi(argv[2]);

	if (!strcmp(argv[1], "t"))
	{
		Serial << "avance de " << val << endl;
		Robot.setup_avancer(val);
	}
	else if (!strcmp(argv[1], "r"))
	{
		Serial << "rotation de " << val << endl;
		Robot.setup_tourner(val);
	}
}

void commande_ecran(int argc, char **argv)
{
	// if (argc == 2)
	// 	ecran.write(ADRESSE_ECRAN, argv[1]);
}

void set_pid(int argc, char **argv)
{
	if (argc != 4)
		return;

	Robot.set_coefs_PID(atof(argv[1]), atof(argv[2]), atof(argv[3]));
}

void set_consigne(int argc, char **argv)
{
	if (argc != 4)
		return;

	Robot.set_consigne(atof(argv[1]), atof(argv[2]));
}