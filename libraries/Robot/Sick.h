#pragma once

#include <stdint.h>

class Sick
{
	public:
		void setup(uint8_t _pin);

		bool is_active();

	private:
		uint8_t pin;
};
