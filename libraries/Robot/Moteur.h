#pragma once

#include "helpers.h"

extern const uint8_t CW;
extern const uint8_t CCW;

class Moteur
{
	public:
		struct Config
		{
			uint8_t pinA, pinB;
			uint8_t pinPWM;
			uint8_t side;
			float wheel_radius; //centimetres
		};

		void setup(Moteur::Config config);

		void consigne(uint8_t sens, uint8_t val);
		void consigne(int speed);

		static bool stop;

	 private:
		uint8_t pinA, pinB, pinPWM;
		float wheel_radius;
};