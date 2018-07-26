#include "Task.h"

void Task::init(int _type, Event *trigger)
{
	type = _type;
	event.registered = nullptr;

	if (trigger != nullptr)
	{
		waitTrigger = true;
		trigger->registered = this;
	}
	else
		waitTrigger = false;
}

bool Task::setup()
{
	switch (type)
	{
	case GOTO:
		break;

	case ACTION:
		if (_setup)
			return _setup(data);
		break;
	}

	return true;
}

bool Task::loop()
{
	if (waitTrigger)
		return true;

	bool ret = false;
	switch (type)
	{
	case GOTO:
		break;

	case ACTION:
		ret = _loop();
	}


	if (!ret && event.registered)
		event.registered->waitTrigger = false;

	return ret;
}