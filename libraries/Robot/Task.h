#pragma once

#include <stdint.h>
#include "vec.h"

struct Event
{
	bool completed;
};

struct Task
{
	enum {GOTO, ACTION, WAIT_TIRETTE, MATCH_TIMER, BARRIER};

	void init(int _type, Event *_trigger);
	bool setup();
	bool loop();
	
	uint8_t type;

	union
	{
		 // GOTO
		struct {
			int x, y;
			float angle;
			uint8_t step;
		};

		// ACTION
		struct {
			int (*_setup)(void*), (*_loop)(void*);
			void* data;
		};

		// WAIT_TIRETTE & MATCH_TIMER
		uint8_t pin;
		unsigned long time, prev;
	};

	bool waiting;
	Event event;
	Event *trigger;
};
