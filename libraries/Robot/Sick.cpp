#include "Sick.h"
#include <Arduino.h>


void Sick::setup(uint8_t _pin)
{
	pin = _pin;
	pinMode(pin, INPUT);
}

bool Sick::is_active()
{
	return digitalRead(pin);
}
