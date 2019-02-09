#include "Actions.h"
#include <TaskQueue.h>

int pinTour = 19;
int pinButee = 28;
int pinsRelais[2] = {24, 26};

int capMonter = 0;
int tourLimite=capMonter;
unsigned long last_interrupt_time = 0;
bool bMotor=true;

static int tourPalet = 15;

static void ascenseur_tour()
{
	static int interval = 30;
	unsigned long interrupt_time = millis();
	if (interrupt_time - last_interrupt_time > interval)
	{
		tourLimite--;
	}
	last_interrupt_time = interrupt_time;
}

static void ascenseur_butee()
{
	bMotor = false;
}

void ascenseur_setup()
{
	pinMode(pinTour, INPUT_PULLUP);
	pinMode(pinButee, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(pinTour), ascenseur_tour, FALLING);

	for (int pin : pinsRelais)
	{
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}

	ascenseur_descendre();
}

void ascenseur_monter()
{
}

void ascenseur_descendre()
{
	Serial << "demarrage descente" << endl;
	digitalWrite(pinsRelais[0], LOW);
	digitalWrite(pinsRelais[1], HIGH);

	while (digitalRead(pinButee) == HIGH)
		;

	Serial << tourLimite << endl;
	
	digitalWrite(pinsRelais[0], HIGH);
	digitalWrite(pinsRelais[1], HIGH);
}