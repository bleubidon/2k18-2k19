#pragma once

#include <Arduino.h>

class Sick
{
	public:
		void setup(uint8_t _pin)
		{
			pin = _pin;
			pinMode(pin, INPUT);
		}

		bool is_active()
		{
			return digitalRead(pin);
		}

		static bool enable;

	private:
		uint8_t pin;
};
