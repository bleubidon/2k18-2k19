#include <Timer.h>
#include <I2CParser.h>

#include "Actions.h"

#define ADRESSE_ECRAN 42

// NOTE Parser: mettre l'option fin de ligne dans la console Arduino pour pouvoir envoyer des commandes

I2CParser ecran;
Parser parser;

Timer timer(500);

void setup()
{
	Serial.begin(9600);

	Serial << "Setup parser..." << endl;
	parser.add("mv", deplacement);
	parser.add("stop", deplacement);
	//parser.add("pid", set_pid);
	//parser.add("consigne", set_consigne);
	//parser.add("ecran", commande_ecran);

	Serial << "Setup Robot..." << endl;
	Robot.setup({
		equipe : GAUCHE,
		pinTirette : 27,
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
			{4, 9, 6, wheel_radius: 3.0f},
			{7, 8, 5, wheel_radius: 3.0f}
		},
		accel_max: 10
	});

	Serial << "Setup ecran..." << endl;
	ecran.setup();
	//requestColor();

	Serial << "Setup task queues..." << endl;
	setup_actions();
	

	Serial << "Setup done !" << endl;
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

	loop_actions();

	loop_ecran();
}

void loop_ecran()
{
	if (timer.on())
	{
		static char command[15];
		sprintf(command, "pos %d %d", (int)Robot.position.pos().x, (int)Robot.position.pos().y);

		ecran.write(ADRESSE_ECRAN, command);
	}
}

// Commands
extern Event starter;
extern TaskQueue test;

void deplacement(int argc, char **argv)
{
	if (strcmp(argv[0], "stop") == 0)
	{
		test.clear();
		Robot.stop();
	}
	else if (strcmp(argv[0], "mv") == 0)
		starter.completed = true;
}

void commande_ecran(int argc, char **argv)
{
	// if (argc == 2)
	// 	ecran.write(ADRESSE_ECRAN, argv[1]);
}
/*
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
*/