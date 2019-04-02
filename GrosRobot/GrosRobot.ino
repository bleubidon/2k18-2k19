#include <I2CParser.h>
#include <DynamixelSerial2.h>

#include "Actions.h"
#include "test.h"
#include "helpers.h"

// NOTE Parser: mettre l'option fin de ligne dans la console Arduino pour pouvoir envoyer des commandes

Parser parser;

void setup()
{
	Serial.begin(9600);
	setup_ecran();

	affichage("Setup...");

	setup_ascenseur();
	setup_actions();

	Robot.setup({
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
		dist : PID(25.f, 0.f, 2.f),
		rot : PID(10.f, 0.f, 0.5f)
	});

	DEBUG(Serial << "Done" << endl);
	clear_ecran();


	parser.add("dist", dist);
	parser.add("rot", rot);
	parser.add("stop", stop);
	parser.add("pid", set_pid);
	parser.add("cycle", do_cycle);
	parser.add("square", exec_square);
	parser.add("test", unit_test);
    parser.add("lcd_print", affichage);
    parser.add("lcd_clear", clear_ecran);
    parser.add("g", set_axg);
	parser.add("d", set_axd);
}

void loop()
{
	parser.loop();

	loop_actions();
	Robot.loop_pid();
}

void affichage(int argc, char **argv)
{
    affichage(argv[1]);
}

void exec_square(int argc, char **argv)
{
	do_square.restart();
}

void set_pid(int argc, char **argv)
{
	if (argc != 5)
		return;
	if (argv[1][0] == '0')
		Robot.dist_pid().set_coefs(atof(argv[2]), atof(argv[3]), atof(argv[4]));
	else
		Robot.rot_pid().set_coefs(atof(argv[2]), atof(argv[3]), atof(argv[4]));
}

void dist(int argc, char **argv)
{
	if (argc == 2)
		Robot.consigne_rel(atof(argv[1]), 0.f);
}

void rot(int argc, char **argv)
{
	if (argc == 2)
		Robot.consigne_rel(0.f, atof(argv[1]));
}

void stop(int argc, char **argv)
{
	Robot.stop();
}

void set_axg(int argc, char **argv)
{
	set_pinces(atoi(argv[1]), -1);
}

void set_axd(int argc, char **argv)
{
	set_pinces(-1, atoi(argv[1]));
}

void do_cycle(int argc, char **argv)
{
	cycle_ascenseur();
}
