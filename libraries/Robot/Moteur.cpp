#include "Moteur.h"

#include <PWM.h>

bool Moteur::stop = false;

void Moteur::setup(Moteur::Config config)
{
	pinA = config.pinA;
	pinB = config.pinB;
	pinPWM = config.pinPWM;

	factor = (config.side == DROITE) * 2 - 1;
	wheel_radius = config.wheel_radius;

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

void Moteur::consigne(int speed)
{
	speed = clamp(-255, speed, 255);
	consigne((speed * factor) < 0, abs(speed));
}