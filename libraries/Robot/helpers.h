#pragma once

#include <Arduino.h>

extern const uint8_t GAUCHE;
extern const uint8_t DROITE;

extern const char endl;

inline int clamp(int _min, int _val, int _max)
{
	return max(_min, min(_val, _max));
}

template <class T>
inline Print &operator<<(Print &obj, T arg)
{
	obj.print(arg);
	return obj;
}

#ifdef NDEBUG
# define DEBUG(x) void;
#else
# define DEBUG(x) x
#endif