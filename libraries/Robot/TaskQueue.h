#pragma once

#include "Task.h"

const uint8_t MAX_QUEUED_TASKS = 32;
const uint8_t MAX_ACTIVE_TASKS = 8;

class TaskQueue
{
	friend class Task;

	public:
		TaskQueue();

		void loop();

		void restart();
		void clear();
		bool finished();

		Event *enqueueGoto(vec dest, int angle);
		Event *enqueueGoto(vec dest, int angle, Event *trigger);
		Event *enqueueAction(int (*action)(void*), int (*setup)(void*), void *data = nullptr, Event *trigger = nullptr);

		Event *enqueueWaitTirette(uint8_t pin, Event *trigger = nullptr);
		Event *enqueueMatchTimer(unsigned long time, Event *trigger = nullptr);

		Event *enqueueBarrier();

		Event *wait_previous();

	private:
		Task queue[MAX_QUEUED_TASKS];
		Task *active[MAX_ACTIVE_TASKS];

		uint8_t queueSize, activeSize;
		uint8_t index, locked;
};
