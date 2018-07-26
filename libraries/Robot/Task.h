#pragma once

struct Task;
struct Event
{
	Task *registered;
};


struct Task
{
	enum {GOTO, TIMER, ACTION, BARRIER};

	void init(int _type, Event *trigger);
	bool setup();
	bool loop();
	
	int type;

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
	bool waitTrigger;
};