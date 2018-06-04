#include "Moteur.h"

#include <PWM.h>

bool Moteur::stop = false;


void Moteur::setup(int _pinA, int _pinB, int _pinPWM)
{
    SetPinFrequencySafe(pinPWM, 20000);

	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
	pinMode(pinPWM, OUTPUT);
	
	digitalWrite(pinA, LOW);
	digitalWrite(pinB, LOW);
}

void Moteur::setup(int _pins[3])
{
	setup(_pins[0], _pins[1], _pins[2]);
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