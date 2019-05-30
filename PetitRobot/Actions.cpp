#include "Actions.h"

#define LAMBDA(cmd) [] (int, char**) { cmd(); }

int robot_stop(void *)
{
	Serial.println("Fin du match");
	Robot.stop();
	Moteur::stop = true;
	return 0;
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

void setup_parser()
{
	parser.add("pid", set_pid);

	parser.add("dist", dist);
	parser.add("rot", rot);
	parser.add("stop", LAMBDA(Robot.stop) );
}
