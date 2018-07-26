#pragma once

#include "Task.h"

const int MAX_QUEUED_TASKS = 64;
const int MAX_ACTIVE_TASKS = 16;

class TaskQueue
{
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

		int queueSize, activeSize;
		int index, locked;
};