#pragma once

#include "Task.h"

const uint8_t MAX_QUEUED_TASKS = 64;
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

		Event *enqueueGoto(int x, int y, int angle, Event *trigger = nullptr);
		Event *enqueueTimer(int time, Event *trigger = nullptr);
		Event *enqueueAction(int (*action)(), int (*setup)(void*), void *data = nullptr, Event *trigger = nullptr);

		void enqueueBarrier();

	private:
		void step();

		Task queue[MAX_QUEUED_TASKS];
		Task *active[MAX_ACTIVE_TASKS];

		uint8_t queueSize, activeSize;
		uint8_t index, locked;
};