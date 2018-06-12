#pragma once
	
#include <Arduino.h>


class Encodeur
{
	public:
		void setup(int _pin, int nb_pas_max, float rayon);

		float getDistance();

		long getCounter();
		void clearCounter();

	private:
		static bool initializedSPI;

		void initCounter();
		void transfer(uint8_t order, uint8_t data);

		int pin;
		float ratio;
};