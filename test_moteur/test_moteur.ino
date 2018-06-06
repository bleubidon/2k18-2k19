#include <Robot.h>
#include <PWM.h>

// #define MODE0
// #define MODE1
// #define MODE2

int inApin[2] = {7, 4}; // INA: Clockwise input
int inBpin[2] = {8, 9}; // INB: Counter-clockwise input
int pwmpin[2] = {5, 6}; // PWM input

#ifdef MODE1
	Moteur m[2];
#endif

void setup()
{
	Serial.begin(9600);
	InitTimersSafe();

	#if defined MODE0
		SetPinFrequencySafe(pwmpin[0], 20000);
		SetPinFrequencySafe(pwmpin[1], 20000);

		for (int i = 0; i < 2; i++)
		{
			pinMode(inApin[i], OUTPUT);
			pinMode(inBpin[i], OUTPUT);
			pinMode(pwmpin[i], OUTPUT);
		}

		for (int i = 0; i < 2; i++)
		{
			digitalWrite(inApin[i], LOW);
			digitalWrite(inBpin[i], LOW);
		}

	#else
		for (int i = 0; i < 2; i++)
			m[i].setup(inApin[i], inBpin[i], pwmpin[i]);
	#endif
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)
{
	#if defined MODE0
		// Set inA[motor]
		if (direct <= 1)
			digitalWrite(inApin[motor], HIGH);
		else
			digitalWrite(inApin[motor], LOW);

		// Set inB[motor]
		if ((direct == 0) || (direct == 2))
			digitalWrite(inBpin[motor], HIGH);
		else
			digitalWrite(inBpin[motor], LOW);

		analogWrite(pwmpin[motor], pwm);

	#elif defined MODE1
		m[motor].consigne(direct, pwm);
	#endif
}

void consigneMoteur(int consigne_vitesse, int consigne_vitesse2)
{
	consigne_vitesse = max(-255, min(consigne_vitesse, 255));
	consigne_vitesse2 = max(-255, min(consigne_vitesse2, 255));

	if (consigne_vitesse > 0)
	{
		motorGo(1, CCW, consigne_vitesse); //1023); //moteur 1
	}
	else
	{
		motorGo(1, CW, -consigne_vitesse);
	}
	if (consigne_vitesse2 > 0)
	{
		motorGo(0, CW, consigne_vitesse2); //1023); //moteur 2
	}
	else
	{
		motorGo(0, CCW, -consigne_vitesse2);
	}
}

void consigneMoteurs2(int consigne_gauche, int consigne_droite)
{
	consigne_gauche = max(-255, min(consigne_gauche, 255));
	consigne_droite = max(-255, min(consigne_droite, 255));

	if (consigne_gauche > 0)
		m[0].consigne(CCW, consigne_gauche);
	else
		m[0].consigne(CW, -consigne_gauche);

	if (consigne_droite > 0)
		m[1].consigne(CW, consigne_droite);
	else
		m[1].consigne(CCW, -consigne_droite);
} 

void loop()
{
	#if defined MODE2
		consigneMoteurs2(50, 50);

	#else
		consigneMoteur2(50, 50);
	#endif
}
