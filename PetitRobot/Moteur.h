#include <Arduino.h>

#define BRAKEVCC 0
#define CW   1
#define CCW  2
#define BRAKEGND 3

class Moteur
{
	public:
		Moteur(int _pinA, int _pinB, int _pinPWM);
		Moteur(int _pins[3]);

		void setup();
		void consigne(int sens, int val);

		static bool stop;

	private:
		int pinA, pinB, pinPWM;
};