#include "Actions.h"
#include <TaskQueue.h>

int robot_stop(void *);

// TODO: Implement sensor management in interrupts

TaskQueue do_square;

void setup_actions()
{
	/*
	do_square.enqueueWaitTirette(27);
	do_square.enqueueBarrier();
	*/

	/*
	do_square.enqueueMatchTimer(90000L); // = 90 secondes
	do_square.enqueueAction(robot_stop, nullptr, nullptr, do_square.wait_previous());
	*/

	// Les angles sont en degrés
	do_square.enqueueGoto(vec(40, 0), 90, nullptr);
	do_square.enqueueGoto(vec(40, 30), 180);
	do_square.enqueueGoto(vec(0, 30), 270);
	do_square.enqueueGoto(vec(0, 0), 0);
}

void loop_actions()
{
	do_square.loop();
}

int robot_stop(void *)
{
	Serial.println("Fin du match");
	Robot.stop();
	Moteur::stop = true;
	return 0;
}
