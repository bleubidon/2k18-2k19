#pragma once

#include <Arduino.h>

#define CW   0
#define CCW  1

extern const uint8_t GAUCHE;
extern const uint8_t DROITE;

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
		int factor;
		float wheel_radius;
};

inline int clamp(int _min, int _val, int _max)
{
	return max(_min, min(_val, _max));
}