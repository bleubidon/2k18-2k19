#pragma once

class Sick
{
	public:
		void setup(int _pin);

		bool active();

	private:
		int pin;
};