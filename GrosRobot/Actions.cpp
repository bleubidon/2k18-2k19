#include "Actions.h"
#include <TaskQueue.h>

int robot_stop(void *);

// TODO: Implement sensor management in interrupts
// TODO: pathfinder
// TODO: handle still opponent

TaskQueue chaos;
TaskQueue monter_rampe;

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
	monter_rampe.enqueueGoto({
        goto_rampe rampe();
        monter_rampe.enqueueTask(&rampe);

        dest: vec(40, 0),
        angle: 90,
        trigger: nullptr,
        on_obstacle: avoid,
        on_start: nullptr,
        setup: nullptr,
        loop: test_done,
        on_finish: nullptr,
    });

    monter_rampe.enqueueAction({
        setup: ouvrir,
        trigger: ready_to_open
        loop: 
    });


    Robot.set_task_queue(chaos);
}

int robot_stop(void *)
{
	Serial.println("Fin du match");
	Robot.stop();
	Moteur::stop = true;
	return 0;
}
