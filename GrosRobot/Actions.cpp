#include <Radio.h>
#include "Actions.h"
#include "test.h"

int robot_stop(void *);

int waitTirette(uint8_t pin, Button& selecteur)
{
	static const char *equipes[] = {"JAUNE", "VIOLETTE"};
	static const char *format = "EQUIPE %s";

	int equipe = DROITE;
	char buf[16];

	sprintf(buf, format, equipe[equipes]);
	affichage(buf, 0);
	affichage("Tirette");

	pinMode(pin, INPUT_PULLUP);

	while (true)
	{
		Serial << "awaiting tirette" << endl;
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
	Serial << "send 102 " << equipe << endl;

	return equipe;
}

int launch_experience(int equipe) {
    byte id = 78;
    Radio radio(35, 37);
    radio.setup(id, 42, 38);
    Message *response;
    do {
        radio.send(102, !equipe ? "jaune" : "violette");
        while (!(response = radio.loop())) {
            delay(100);
            Serial << "ping experience" << endl;
        }
    }
    while (strcmp(response->text, "OK"));
    Serial << "Experience lancee OK" << endl;

    return 0;
}

void fetch_atom(int argc, char **argv)
{
	if (argc < 2) // S'arreter si aucun palet n'a ete detecte
	{
		chaos_zone_state = NO_MORE_ATOM;
		return;
	}
	else
		chaos_zone_state = MORE_ATOM;

	char to_display[16];
	snprintf(to_display, sizeof(to_display), "R:%s; D:%s", argv[1], argv[2]);
	affichage(to_display);

	float dist_init = Robot.pos().dist();
	float rot_init = Robot.pos().rot();
	float angle_error = atof(argv[1]);
	float dist_error = atof(argv[2]);

	// Le robot s'oriente en direction du palet detecte
	Robot.consigne_rel(0.f, angle_error);
	while(Robot.loop_pid());

	// Renvoi d'une requete tant que l'erreur angulaire est trop elevee
	if (abs(angle_error) >= 2)
		Serial << "request_2" << endl;
	else
	{
		// Le robot avance jusqu'a au plus la distance au palet calculee
		Robot.consigne_rel(dist_error, 0.f);
		while(Robot.loop_pid())
		{
			// Le robot s'arrete s'il detecte que le palet est en butee
			if (digitalRead(pinPalet) == LOW) {
				Robot.stop();
				break;
			}
		}

		// Le robot revient a sa position initiale
		Robot.consigne(0.f, rot_init);
		while(Robot.loop_pid());
		Robot.consigne(dist_init, 0.f);
		while(Robot.loop_pid());
	}
}

int robot_stop(void *)
{
	Serial.println("Fin du match");
	Robot.stop();
	Moteur::stop = true;
	return 0;
}

void set_pid(int argc, char **argv)
{
	if (argc != 5)
		return;
	if (argv[1][0] == '0')
		Robot.dist_pid().set_coefs(atof(argv[2]), atof(argv[3]), atof(argv[4]));
	else
		Robot.rot_pid().set_coefs(atof(argv[2]), atof(argv[3]), atof(argv[4]));
}

void dist(int argc, char **argv)
{
	if (argc == 2)
		Robot.consigne_rel(atof(argv[1]), 0.f);
}

void rot(int argc, char **argv)
{
	if (argc == 2)
		Robot.consigne_rel(0.f, atof(argv[1]));
}

void set_axg(int argc, char **argv)
{
	set_pinces(atoi(argv[1]), -1);
}

void set_axd(int argc, char **argv)
{
	set_pinces(-1, atoi(argv[1]));
}

void open_pliers(int argc, char **argv)
{
    set_pinces(opened_pliers_values[0], opened_pliers_values[1]);
}

#define LAMBDA(cmd) [] (int, char**) { cmd(); }

void setup_parser()
{
	parser.add("pid", set_pid);
	parser.add("test", unit_test);

	parser.add("dist", dist);
	parser.add("rot", rot);
	parser.add("stop", LAMBDA(Robot.stop) );

	parser.add("lcd_print", [] (int, char **argv) { affichage(argv[1]); } );
	parser.add("lcd_clear", LAMBDA(clear_ecran) );

	parser.add("up", LAMBDA(montee_plateau) );
	parser.add("down", LAMBDA(descente_plateau) );
	parser.add("cycle", LAMBDA(cycle_ascenseur) );

	parser.add("ax", [] (int, char **argv) {
		set_pinces(atoi(argv[1]), atoi(argv[2]));
	} );
	parser.add("axg", set_axg);
	parser.add("axd", set_axd);

	parser.add("open", open_pliers);

	parser.add("rpi_response", fetch_atom);
}
