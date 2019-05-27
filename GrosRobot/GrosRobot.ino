#include "Actions.h"

// NOTE Parser: mettre l'option fin de ligne dans la console Arduino pour
// pouvoir envoyer des commandes

// TODO: Setup initial position and orientation

const int pinTirette = 65;
const int pinBouton = 49;

int equipe;

Parser parser;
Button button;

byte chaos_zone_state;

void setup()
{
	Serial.begin(9600);
	setup_ecran();

	affichage("Setup...");

	setup_ascenseur();
	setup_parser();

	button.setup(pinBouton);

	equipe = waitTirette(pinTirette, button);

	Robot.setup({
		odometrie : {
			position: POS_SYM(10, 10, equipe),
			angle: ANGLE_SYM(90, equipe),
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
		sicks: {66, 67, 68, 69},
		min_speed: 15,
		max_speed: 255,
		dist : PID(25.f, 0.f, 2.f),
		rot : PID(10.f, 0.f, 0.5f)
	});

	affichage("Debut du match !", 1, true);
}

/*
**
** Interface deplacement:
**
** Fonctions
**  - translate
**  - rotate
**  - go_to
**  - look_at
**
** Arguments:
**  - angle/distance/pos relatif/absolu
**  - bool blocking = true
 */

void loop()
{
	/*
	// Recuperer premier palet devant le spawn
	Robot.translate(10);
	cycle_ascenseur();

	// Avancer jusqua la zone de chaos
	Robot.go_to(vec(x, x)); // pos de la zone de chaos
	Robot.look_at(vec(x, x)); // pos du centre de la zone de chaos

	// Recuperer les palets
	do
	{
		chaos_zone_state = WAIT_RASP;
		Serial << "request" << endl;
		while (chaos_zone_state == WAIT_RASP)
			parser.loop();
	}
	while (chaos_zone_state != NO_MORE_ATOM);
		
	// Revenir a la zone de depart
	Robot.go_to(vec(x, x)); // pos de la zone de depart
	open_pinces();
	Robot.translate(10); // pousse les palets
	Robot.translate(-10);

	// Monter la rampe
	Robot.go_to(vec(x, x)); // pos de devant la rampe
	Robot.go_to(vec(x, x)); // pos du haut de la rampe
	open_pinces();
	Robot.translate(-10);
	close_pinces();
	Robot.translate(10); // pousse les palets

	Robot.stop();
	*/

	parser.loop();
	Robot.loop_pid();

	if (button.loop() == State::Released)
		Serial << "request" << endl; // Send request to rpi
}
