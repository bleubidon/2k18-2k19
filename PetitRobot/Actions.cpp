#include "Actions.h"

int robot_stop(void*);

// TODO: Implement sensor management in interrupts

//TaskQueue do_square;

void setup_actions()
{
}

void loop_actions()
{
}

int robot_stop(void*)
{
	Serial.println("Fin du match");
	Robot.stop();
	Moteur::stop = true;
	return 0;
}
