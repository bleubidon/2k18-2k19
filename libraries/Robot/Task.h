#pragma once

#include <stdint.h>

struct Event
{
	bool completed;
};

struct Task
{
	enum {GOTO, TIMER, ACTION, BARRIER};

	void init(int _type, Event *_trigger);
	bool setup();
	bool loop();
	
	uint8_t type;

	union
	{
		 // GOTO
		struct {
			int x, y, angle;
		};

		// TIMER
		unsigned long duration;

		// ACTION
		struct {
			int (*_setup)(void*), (*_loop)();
			void* data;
		};
	};

	Event event;
	Event *trigger;
};