#include "Task.h"
#include "Robot.h"
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
		return Robot.setup_goto(x, y, angle);

	case ACTION:
		if (_setup)
			return _setup(data);
		break;

	case BARRIER:
		TaskQueue *tq = (TaskQueue *)data;
		tq->locked++;
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
	case GOTO:
		return Robot.loop_goto();

	case ACTION:
		return _loop(data);

	case BARRIER: {
		TaskQueue *tq = (TaskQueue *)data;
		if (tq->activeSize == tq->locked) // No active task left
		{
			tq->locked--;
			return false;
		}
		return true;
	}

	default:
		return false;
	}
}