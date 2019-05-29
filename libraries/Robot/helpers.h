#pragma once

//#define NDEBUG	// Define to disable debug messages
//#define NLOG		// Define to disable log messages

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
# define DEBUG(x)
#else
# define DEBUG(x) x
#endif

#undef NLOG

#ifdef NLOG
# define LOG(x)
#else
# define LOG(x) x
#endif
