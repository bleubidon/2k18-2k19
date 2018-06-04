#include "Moteur.h"

#include <PWM.h>

bool Moteur::stop = false;

Moteur::Moteur(int _pinA, int _pinB, int _pinPWM):
	pinA(_pinA), pinB(_pinB), pinPWM(_pinPWM)
{
    SetPinFrequencySafe(pinPWM, 20000);
}

Moteur::Moteur(int _pins[3]):
	Moteur(_pins[0], _pins[1], _pins[2])
{ }


void Moteur::setup()
{
	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
	pinMode(pinPWM, OUTPUT);
	
	digitalWrite(pinA, LOW);
	digitalWrite(pinB, LOW);
}

void Moteur::consigne(int sens, int val)
{
    if (stop)
        return;

	if (sens <=4)
	{
		if (sens <=1)
			digitalWrite(pinA, HIGH);
		else
			digitalWrite(pinA, LOW);

		// Set inB[motor]
		if ((sens==0)||(sens==2))
			digitalWrite(pinB, HIGH);
		else
			digitalWrite(pinB, LOW);


		analogWrite(pinPWM, val);
	}
}