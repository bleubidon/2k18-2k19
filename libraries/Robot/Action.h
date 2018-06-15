#pragma once

enum class ActionType {GOTO, ROTATE, EFFECTEUR, WAIT};

struct Action
{
	ActionType type;

	union
	{
		 // GOTO
		struct {
			int x, y;
		};
		
		// ROTATE
		int alpha;

		// EFFECTEUR
		void (*action)(void);

		// WAIT
		unsigned long duration;
	};
};

inline Action GOTO(int x, int y)
{
	return Action {
		ActionType::GOTO,
		{ x, y }
	};
}

inline Action ROTATE(int alpha)
{
	return Action {
		ActionType::ROTATE,
		{ alpha }
	};
}

inline Action EFFECTEUR(void (*action)(void))
{
	return Action {
		ActionType::EFFECTEUR,
		{ action }
	};
}

inline Action WAIT(unsigned long duration)
{
	return Action {
		ActionType::WAIT,
		{ duration }
	};
}