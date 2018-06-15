#pragma once

#include <Arduino.h>

#define CW   1
#define CCW  2


class Moteur
{
	public:
		struct Config
		{
			int pinA, pinB;
			int pinPWM;
		};

		void setup(Moteur::Config config);

		void consigne(uint8_t sens, uint8_t val);

		static bool stop;

	private:
		int pinA, pinB, pinPWM;
};