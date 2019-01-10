#include "TaskQueue.h"

TaskQueue::TaskQueue()
{
	clear();
}

void TaskQueue::restart()
{
	index = 0;
	locked = false;
	activeSize = 0;

	for (int i(0); i < queueSize; i++)
	{
		queue[i].waiting = true;
		queue[i].event.completed = false;
	}
}

void TaskQueue::clear()
{
	index = 0;
	locked = false;
	queueSize = activeSize = 0;
}

bool TaskQueue::finished()
{
	return ((activeSize == 0) && (index == queueSize));
}

void TaskQueue::loop()
{
	if (!locked &&
		index < queueSize &&
		activeSize < MAX_ACTIVE_TASKS)
		active[activeSize++] = queue + index++;

	for (int i(0); i < activeSize; i++)
	{
		if (!active[i]->loop())
		{
			active[i]->event.completed = true;
			active[i] = active[--activeSize];
		}
	}	
}

Event *TaskQueue::enqueueGoto(vec dest, int angle)
{
	return enqueueGoto(dest, angle, wait_previous());
}

Event *TaskQueue::enqueueGoto(vec dest, int angle, Event *trigger)
{
	Task &task = queue[queueSize++];
	task.init(Task::GOTO, trigger);

	task.x = dest.x;
	task.y = dest.y;
	task.angle = angle;

	return &task.event;
}

Event *TaskQueue::enqueueAction(int (*action)(void*), int (*setup)(void*), void *data, Event *trigger)
{
	if (action == nullptr)
		return nullptr;

	Task &task = queue[queueSize++];
	task.init(Task::ACTION, trigger);

	task._loop = action;
	task._setup = setup;
	task.data = data;

	return &task.event;
}

Event *TaskQueue::enqueueWaitTirette(uint8_t pin, Event *trigger)
{
	Task &task = queue[queueSize++];
	task.init(Task::WAIT_TIRETTE, trigger);

	task.pin = pin;

	return &task.event;
}

Event *TaskQueue::enqueueMatchTimer(unsigned long time, Event *trigger)
{
	Task &task = queue[queueSize++];
	task.init(Task::MATCH_TIMER, trigger);

	task.time = time;

	return &task.event;
}

Event *TaskQueue::enqueueBarrier()
{
	Task &task = queue[queueSize++];
	task.init(Task::BARRIER, nullptr);

	task.data = this;

	return &task.event;
}

Event *TaskQueue::wait_previous()
{
	return &queue[queueSize - 1].event;
}
