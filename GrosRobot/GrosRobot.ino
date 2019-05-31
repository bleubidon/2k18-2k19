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

	delay(1000);

	setup_ascenseur();
	setup_parser();

	button.setup(pinBouton);

	int sicks[] = {66, 67, 68, 69};

	Robot.setup({
		odometrie : {
			position: POS_SYM(0, 0, equipe),
			angle: ANGLE_SYM(0, equipe),
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
		dist : PID(10.f, 0.f, 5.f), //25.f, 0.f, 2.f)
		rot : PID(7.0f, 0.f, 2.0f) //10.f, 0.f, 0.5f
	});

	/*Robot.translate(10);
	delay(100);
	Robot.translate(-10);*/

	equipe = waitTirette(pinTirette, button);

	Robot.start();
	affichage("Debut du match !", 1, true);
    launch_experience(equipe);
}

void loop()
{
    parser.loop();

    int coef_de_sym = equipe ? 1: -1;

    set_pinces(opened_pliers_values[GAUCHE], opened_pliers_values[DROITE]);
    descente_plateau();

    //côté violet
    // Positionnement vers palet bluenium
    Robot.go_to(vec(30, 0 * coef_de_sym));
    Serial << "NEXT MOVE" << endl;
    delay(500);
    Robot.look_at(vec(30, -10 * coef_de_sym));
    Serial << "NEXT MOVE" << endl;

    // Attrape palet bluenium
    Robot.go_to(vec(30, -25 * coef_de_sym));
    cycle_ascenseur();

    // Attrape palet greenium
    Robot.rotate(-90.f * coef_de_sym);
//    Robot.go_to(vec(0, -25 * coef_de_sym));

    // Depose palet bluenium avec palet redium et recule (safe strat)
    Robot.go_to(vec(-15, -25 * coef_de_sym));
    Serial.println("backward");
    Robot.go_to_bkwd(vec(15, -25));

    // Detour pour pousser palets dans redium
    Robot.go_to(vec(-30, -60 * coef_de_sym));

    // Pousser palets dans redium
    Robot.go_to(vec(-30, -10 * coef_de_sym));

    set_pinces(opened_pliers_values[GAUCHE], opened_pliers_values[DROITE]);
    Robot.go_to_bkwd(vec(-30, -20 * coef_de_sym));

    affichage("Score : 53 pts");
    while(1)
    ;
}
