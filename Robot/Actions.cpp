#include "Actions.h"
#include <TaskQueue.h>

// TODO: Implement position update in a TaskQueue
// TODO: Implement sensor management in interrupts

Event starter{false};
TaskQueue test;
TaskQueue butiner_abeille;

void setup_actions()
{
	test.enqueueWaitTirette(&starter);
	test.enqueueBarrier();

	test.enqueueMatchTimer();
	test.enqueueGoto(10, 10, -40);

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
