// #include <Timer.h>
#include <Robot.h>
#include <Parser.h>

#include "helpers.h"

// NOTE Parser: mettre l'option fin de ligne dans la console Arduino pour pouvoir envoyer des commandes

Parser parser;

void setup()
{
	Serial.begin(9600);

	parser.add("dist", dist);
	parser.add("rot", rot);
	parser.add("stop", stop);
	parser.add("pid", set_pid);

	c_Robot::Config conf = {
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
			{4, 9, 6, wheel_radius: 3.0f, GAUCHE},
			{7, 8, 5, wheel_radius: 3.0f, DROITE}
		},
		sicks: {36, 38},
		min_speed: 10,
		max_speed: 255,
		dist : PID(0.f, 0.f, 0.f),
		rot : PID(10.f, 0.f, 0.f)
	};
	Robot.setup(conf);

	//setup_actions();
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
