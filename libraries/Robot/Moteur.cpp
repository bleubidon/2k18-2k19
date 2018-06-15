#include "Moteur.h"

#include <PWM.h>

bool Moteur::stop = false;


void Moteur::setup(Moteur::Config config)
{
	pinA = config.pinA;
	pinB = config.pinB;
	pinPWM = config.pinPWM;

	SetPinFrequencySafe(pinPWM, 20000);

	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
	pinMode(pinPWM, OUTPUT);
	
	digitalWrite(pinA, LOW);
	digitalWrite(pinB, LOW);
}

void Moteur::consigne(uint8_t sens, uint8_t val)
{
	if (stop)
		return;

	digitalWrite(pinA, sens == CW);
	digitalWrite(pinB, sens == CCW);

	analogWrite(pinPWM, val);
}