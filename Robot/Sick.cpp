#include "Sick.h"
    
#include "Arduino.h"


void Sick::setup(int _pin)
{
	pin = _pin;
	pinMode(pin, INPUT);
}

bool Sick::active()
{
	return digitalRead(pin);
}