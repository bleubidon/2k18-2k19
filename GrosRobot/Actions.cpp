#include "Actions.h"

int robot_stop(void *);

// TODO: Implement sensor management in interrupts
// TODO: pathfinder
// TODO: evitement obstacles mobiles (ex: robot adverse)

void waitTirette(uint8_t pin)
{
	affichage("Tirette");

	while (true)
	{
		if (digitalRead(pin) == HIGH)
			break;

		delay(500);
	}

	affichage("Debut du match !");
}

void setup_actions()
{
}

int robot_stop(void *)
{
	Serial.println("Fin du match");
	Robot.stop();
	Moteur::stop = true;
	return 0;
}
