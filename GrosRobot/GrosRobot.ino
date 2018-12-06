#include <Timer.h>
#include <I2CParser.h>

#include "Actions.h"
#include "helpers.h"

// NOTE Parser: mettre l'option fin de ligne dans la console Arduino pour pouvoir envoyer des commandes

Parser parser;

Timer timer(500);

void setup()
{
	Serial.begin(9600);

	Serial << "Setup parser..." << endl;
	parser.add("mv", deplacement);

	Serial << "Setup Robot..." << endl;
	Robot.setup({
		equipe : GAUCHE,
		pinTirette : 27,
		dureeMatch : 90000L, // = 90 secondes

		odometrie : {
			mode : DOUBLE_CODEUSE,
			{{
				gauche : {
					pin : 10,
					radius : 3.5f,
					step_per_round : 20000
				},
				droite : {
					pin : 11,
					radius : 3.5f,
					step_per_round : 20000
				},
				ecart_entre_roues : 28.0f
			}}
		},
		moteurs : {
			{4, 9, 6, wheel_radius: 3.25f, GAUCHE},
			{7, 8, 5, wheel_radius: 3.25f, DROITE}
		},
		accel_max: 10
	});

	Serial << "Setup task queues..." << endl;
	setup_actions();
	

	Serial << "Setup done !" << endl;
	
	//Robot.setup_avancer(20);
	
}

int puiss = 0;

void loop()
{
	parser.loop();

	//loop_actions();

	Robot.loop_pid();
}

// Commands
extern Event starter;
extern TaskQueue test;

void deplacement(int argc, char **argv)
{
		puiss = atoi(argv[1]);
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
