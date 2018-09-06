#pragma once
	
#include <Arduino.h>

class Codeuse
{
	public:
		struct Config
		{
			uint8_t pin;
			float radius; //centimetres
			int step_per_round;
		};

		void setup(Codeuse::Config config);

		float getDistance() const;
		long getCounter() const;
		
		void clearCounter();

	private:
		static bool initializedSPI;

		void initCounter();
		void transfer(uint8_t order, uint8_t data);

		uint8_t pin;
		float ratio;
};