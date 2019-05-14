#include "GrosRobot.h"
#include <I2CParser.h>
#include <DynamixelSerial2.h>

#include "Actions.h"
#include "test.h"
#include "helpers.h"

#define LAMBDA(cmd) [] (int, char**) { cmd(); }

// NOTE Parser: mettre l'option fin de ligne dans la console Arduino pour pouvoir envoyer des commandes

Parser parser;
int buttonState = 1, buttonState_prev = buttonState;
unsigned int buttonTriggerTimein = 300; // in ms
unsigned long buttonTimer;

void setup()
{
	Serial.begin(9600); //Raspberry Serial communication
	setup_ecran();

	affichage("Setup...");

	setup_ascenseur();
	setup_actions();

	Robot.setup({
		odometrie : {
			mode : DOUBLE_CODEUSE,
			{{
				gauche : {
					pin : 10,
					radius : 3.5f,
					step_per_round : 20000
				},
				droite : {
					pin : 11,
					radius : 3.5f,
					step_per_round : 20000
				},
				ecart_entre_roues : 28.0f
			}}
		},
		moteurs : {
			{4, 9, 6, wheel_radius : 3.25f, GAUCHE},
			{7, 8, 5, wheel_radius : 3.25f, DROITE}
		},
		min_speed: 15,
		max_speed: 255,
		dist : PID(25.f, 0.f, 2.f),
		rot : PID(10.f, 0.f, 0.5f)
	});

	DEBUG(Serial << "Done" << endl);
	clear_ecran();

	parser.add("pid", set_pid);
	parser.add("square", LAMBDA(do_square.restart) );
	parser.add("test", unit_test);

	parser.add("dist", dist);
	parser.add("rot", rot);
	parser.add("stop", LAMBDA(Robot.stop) );

	parser.add("lcd_print", [] (int, char **argv) { affichage(argv[1]); } );
	parser.add("lcd_clear", LAMBDA(clear_ecran) );

	parser.add("up", LAMBDA(montee_plateau) );
	parser.add("down", LAMBDA(descente_plateau) );
	parser.add("cycle", LAMBDA(cycle_ascenseur) );

	parser.add("ax", [] (int, char **argv) { set_pinces(atoi(argv[1]), atoi(argv[2])); } );
	parser.add("axg", set_axg);
	parser.add("axd", set_axd);

	parser.add("rpi_response", handle_rpi_response);

	buttonTimer = millis();
}

void loop()
{
	parser.loop();
	loop_actions();
	Robot.loop_pid();

	buttonState = digitalRead(pinBouton);
	if (buttonState == 0 && buttonState != buttonState_prev && millis() - buttonTimer > buttonTriggerTimein) {  // if button is pressed and timein is up
		buttonTimer = millis();
		Serial.println("request");  // Send request to rpi
	}
	buttonState_prev = buttonState;
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

void handle_rpi_response(int argc, char **argv)
{
	if (argc < 2) // S'arreter si aucun palet n'a ete detecte
		return;

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
		Serial.println("request_2");

	else {
		// Le robot avance jusqu'a au plus la distance au palet calculee
		Robot.consigne_rel(dist_error, 0.f);
		while(Robot.loop_pid()) {
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

		// Renvoi d'une requete pour recuperer le prochain palet
		Serial.println("request"); 
	}
}
