#include "Actions.h"
#include <TaskQueue.h>

int robot_stop(void*);

// TODO: Implement position update in a TaskQueue ?
// TODO: Implement sensor management in interrupts

Event starter{false};
TaskQueue test;
TaskQueue butiner_abeille;

void setup_actions()
{
	test.enqueueWaitTirette(&starter);
	test.enqueueBarrier();

	test.enqueueMatchTimer();
	test.enqueueAction(robot_stop, nullptr, nullptr, test.wait_previous());

	// Les angles sont en degrés
	test.enqueueGoto(40, 0, 90);
	test.enqueueGoto(40, 90, 180, test.wait_previous());
	test.enqueueGoto(0, 90, 270, test.wait_previous());
	test.enqueueGoto(0, 0, 0, test.wait_previous());



/*
	butiner_abeille.clear();

	butiner_abeille.enqueueGoto(12, 8, 45); // x, y, angle
	butiner_abeille.enqueueBarrier();
	butiner_abeille.enqueueAction(lever_bras, setup_bras, NULL);
*/
}

void loop_actions()
{
	test.loop();
}

int robot_stop(void*)
{
	Serial.println("Fin du match");
	Robot.stop();
	Moteur::stop = true;
	return 0;
}

int setup_bras(void *)
{
	Serial.println("Initialisation du bras");
	return 1;
}

int lever_bras(void *)
{
	Serial.println("Et je leve le bras");
	return 0;
}
