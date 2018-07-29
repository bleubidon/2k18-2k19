#include "Task.h"
#include "TaskQueue.h"

void Task::init(int _type, Event *_trigger)
{
	type = _type;

	if (_trigger != nullptr)
		trigger = _trigger;
	else
		_trigger = nullptr;
}

bool Task::setup()
{
	event.completed = false;

	switch (type)
	{
	case GOTO:
		break;

	case ACTION:
		if (_setup)
			return _setup(data);
		break;

	case BARRIER:
		TaskQueue *tq = (TaskQueue*)data;
		tq->locked++;
		break;
	}

	return true;
}

bool Task::loop()
{
	if (!trigger->completed)
		return true;

	bool ret = false;
	switch (type)
	{
	case GOTO:
		break;

	case ACTION:
		ret = _loop();
		break;

	case BARRIER:
		TaskQueue *tq = (TaskQueue*)data;
		ret = tq->activeSize;
		if (ret) // No active tasks left
			tq->locked--;
		break;
	}


	event.completed = !ret;
	return ret;
}