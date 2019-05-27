#include "Moteur.h"

#include <PWM.h>

const uint8_t CW = 0;
const uint8_t CCW = 1;

bool Moteur::stop = false;

void Moteur::setup(Moteur::Config config)
{
	pinA = config.pinA;
	pinB = config.pinB;
	pinPWM = config.pinPWM;
	side = config.side;

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

	if (side)
		sens = !sens;
	digitalWrite(pinA, sens == CW);
	digitalWrite(pinB, sens == CCW);

	analogWrite(pinPWM, val);
}

void Moteur::consigne(int speed)
{
	speed = clamp(-255, speed, 255);
	consigne(speed > 0, abs(speed));
}
