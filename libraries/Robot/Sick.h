#pragma once

#include <stdint.h>

class Sick
{
	public:
		void setup(uint8_t _pin);

		bool active();

	private:
		uint8_t pin;
};