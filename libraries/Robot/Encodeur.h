#pragma once
	
#include <Arduino.h>


class Encodeur
{
	public:
		void setup(int _pin);

		long getCounter();
		void clearCounter();

	private:
		static bool initialized;

		void initCounter();
		void transfer(uint8_t order, uint8_t data);

		int pin;
};