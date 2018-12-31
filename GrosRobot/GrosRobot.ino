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

	parser.add("dist", dist);
	parser.add("rot", rot);
	parser.add("pid", set_pid);
	parser.add("stop", stop);

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
		    {4, 9, 6, wheel_radius : 3.25f, GAUCHE},
		    {7, 8, 5, wheel_radius : 3.25f, DROITE}
		},
		dist: PID(1.f, 0.0f, 0.0f),
		rot: PID(1.f, 0.0f, 0.0f)
	});

	setup_actions();
}

void loop()
{
	parser.loop();

	//loop_actions();

	Robot.loop_pid();
}

void set_pid(int argc, char **argv)
{
	if (argc != 5)
		return;
	if (argv[1][0] == '0')
		Robot.dist.set_coefs(atof(argv[0]), atof(argv[1]), atof(argv[2]));
	else
		Robot.rot.set_coefs(atof(argv[0]), atof(argv[1]), atof(argv[2]));
}

void dist(int argc, char **argv)
{
	if (argc == 2)
		Robot.consigne(atof(argv[1]), 0);
}

void rot(int argc, char **argv)
{
	if (argc == 2)
		Robot.consigne(0, atof(argv[1]));
}

void stop(int argc, char **argv)
{
	Robot.stop();
}
