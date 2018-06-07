#pragma once

#include <Arduino.h>

#define BRAKEVCC 0
#define CW   1
#define CCW  2
#define BRAKEGND 3

class Moteur
{
	public:
		void setup(int _pinA, int _pinB, int _pinPWM);
		void setup(int _pins[3]);

		void consigne(uint8_t sens, uint8_t val);

		static bool stop;

	private:
		int pinA, pinB, pinPWM;
};