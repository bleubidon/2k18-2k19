#include "Actions.h"

// NOTE Parser: mettre l'option fin de ligne dans la console Arduino pour
// pouvoir envoyer des commandes

// TODO: Setup initial position and orientation

//#define mode_parser

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

	delay(1000);

	setup_ascenseur();
	setup_parser();

	button.setup(pinBouton);

	int sicks[] = {66, 67, 68, 69};

	Robot.setup({
		odometrie : {
			position: vec(0, 0),
			angle: 0,
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
		num_sicks: 4,
		sicks: sicks,
		dureeMatch: 90000000L,
		min_speed: 20,
		max_speed: 70,
		dist : PID(10.f, 0.f, 5.f),
		rot : PID(7.0f, 0.f, 2.0f)
	});

#ifndef mode_parser
	Robot.translate(10);
	delay(100);
	Robot.translate(-10);
#endif

	equipe = waitTirette(pinTirette, button);

	Robot.start();
	affichage("Debut du match !", 1, true);

	launch_experience(equipe);
}

void loop()
{
#ifdef mode_parser
	while(1)
	{parser.loop();
	Robot.loop_pid();
	}
#endif

    set_pinces(opened_pliers_values[GAUCHE], opened_pliers_values[DROITE]);
    descente_plateau();

    //côté violet
    // Positionnement vers palet bluenium
    Robot.go_to(POS_SYM(105, 25));
    Serial << "NEXT MOVE" << endl;
    delay(500);
    Serial << "NEXT MOVE" << endl;

    // Attrape palet bluenium
    Robot.go_to(POS_SYM(105, 50));
    cycle_ascenseur();

    // Attrape palet greenium
    Robot.go_to(POS_SYM(60, 50));

    // Depose palet bluenium et greenium avec palet redium et recule (safe strat)
    set_pinces(opened_pliers_values[GAUCHE], opened_pliers_values[DROITE]);
    Serial.println("backward");
    Robot.go_to_bkwd(POS_SYM(90, 50));

    // Detour pour pousser palets dans redium
    Robot.go_to(POS_SYM(45, 85));

    // Pousser palets dans redium
    Robot.go_to(POS_SYM(45, 25));

    Robot.go_to_bkwd(POS_SYM(45, 50));

    // Récup palets chaos:
    Robot.go_to(POS_SYM(45, 134));
    Robot.go_to(POS_SYM(129, 134));
    // Pousser palets dans redium
    Robot.go_to(POS_SYM(50, 40));

    affichage("Sur la feuille !");
    while(1)
    ;
}
