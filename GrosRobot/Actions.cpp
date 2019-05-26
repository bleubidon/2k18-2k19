#include "Actions.h"

int robot_stop(void *);

// TODO: Implement sensor management in interrupts
// TODO: pathfinder
// TODO: evitement obstacles mobiles (ex: robot adverse)

int waitTirette(uint8_t pin, Button& selecteur)
{
	static const char *equipes[] = {"VIOLETTE", "JAUNE"};
	static const char *format = "EQUIPE %s";

	int equipe = GAUCHE;
	char buf[16];

	sprintf(buf, format, equipe[equipes]);
	affichage(buf, 0);
	affichage("Tirette");

	pinMode(pin, INPUT_PULLUP);

	while (true)
	{
		if (selecteur.loop() == State::Pressed)
		{
			equipe = 1 - equipe;
			sprintf(buf, format, equipe[equipes]);
			affichage(buf, 0);
		}
		if (digitalRead(pin) == HIGH)
			break;
	}

	// Send team via raspberry to id 3 (experience)
	Serial << "send 3 " << equipe << endl;

	affichage("Debut du match !", 1, true);
	return equipe;
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
