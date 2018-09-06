#include "Actions.h"
#include <TaskQueue.h>

// TODO: TaskQueue must be handled by Robot class
// TODO: Implement game timer in a TaskQueue
// TODO: Implement position update in a TaskQueue
// TODO: Implement sensor management in interrupts

TaskQueue butiner_abeille;

void setup_actions()
{
	butiner_abeille.clear();

	butiner_abeille.enqueueTimer(30); // useless for now
	butiner_abeille.enqueueGoto(12, 8, 45); // x, y, angle
	butiner_abeille.enqueueBarrier();
	butiner_abeille.enqueueAction(lever_bras, setup_bras, NULL);
	
	//Robot.register_action(butiner_abeille);
}

void loop_actions()
{
	butiner_abeille.loop();
}

int setup_bras(int *data)
{
	Serial.println("Initialisation du bras");
	return 1;
}

int lever_bras()
{
	Serial.println("Et je leve le bras");
	return 0;
}
