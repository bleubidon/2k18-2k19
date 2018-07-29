#pragma once

#include <Arduino.h>

#define CW   1
#define CCW  2

class Moteur
{
	public:
		struct Config
		{
			uint8_t pinA, pinB;
			uint8_t pinPWM;
		};

		void setup(Moteur::Config config);

		void consigne(uint8_t sens, uint8_t val);

		static bool stop;

	private:
		uint8_t pinA, pinB, pinPWM;
};