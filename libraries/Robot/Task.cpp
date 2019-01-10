#include "Task.h"
#include "Robot.h"
#include "helpers.h"
#include "TaskQueue.h"

void Task::init(int _type, Event *_trigger)
{
	type = _type;
	waiting = true;
	event.completed = false;
	trigger = _trigger;
}

bool Task::setup()
{
	switch (type)
	{
	case GOTO:
		step = 0;
		break;

	case ACTION:
		if (_setup)
			return _setup(data);
		break;

	case WAIT_TIRETTE:
		pinMode(pin, INPUT_PULLUP);
		prev = millis();
		break;

	case MATCH_TIMER:
		prev = millis();
		break;

	case BARRIER:
		TaskQueue *tq = (TaskQueue *)data;
		tq->locked = true;
		break;
	}

	return true;
}

bool Task::loop()
{
	if (waiting)
	{
		if (trigger && !trigger->completed)
			return true;
		waiting = false;
		return setup();
	}
	switch (type)
	{
	case GOTO: {
		/*
		 * even steps represent new consigne
		 * odd steps represent waiting for end of consigne
		 * The three steps of a goto are:
		 *  - Rotate toward destination
		 *  - Move forward to destination
		 *  - Rotate to final orientation
		*/

		if (step == 0) {
			vec pos = Robot.pos().pos();
			vec dir(x - pos.x, y - pos.y);
			Robot.consigne_rel(0, dir.angle() - Robot.pos().rot()), step++;
		} else if (step == 2) {
			vec dest(x, y);
			Robot.consigne_rel(vec::dist(Robot.pos().pos(), dest), 0), step++;
		} else if (step == 4)
			Robot.consigne_rel(0, angle - Robot.pos().rot()), step++;

		// robot reached destination, go to next step
		if (!Robot.loop_pid() && (step % 2))
			step++;
		return (step != 6); // reaching step 6 means we are done
	}

	case ACTION:
		return _loop(data);

	case WAIT_TIRETTE: {
		unsigned long now = millis();
		if (now - prev > 1000)
			return true;
		Serial << "En attente de la tirette sur la pin " << pin << endl;
		return (digitalRead(pin) == LOW);
	}

	case MATCH_TIMER:
		return (millis() - prev < time);

	case BARRIER: {
		TaskQueue *tq = (TaskQueue *)data;
		if (tq->activeSize == 1) // No other active task left
			return (tq->locked = false);
		return true;
	}

	default:
		return false;
	}
}
