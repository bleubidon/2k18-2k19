// #include <Timer.h>
#include <Robot.h>
#include "Actions.h"

#include "helpers.h"

// NOTE Parser: mettre l'option fin de ligne dans la console Arduino pour pouvoir envoyer des commandes

Parser parser;

void setup()
{
	Serial.begin(9600);

	setup_parser();

	c_Robot::Config conf = {
		odometrie : {
			position: vec(0, 0),
			angle: 0,
			mode : CODEUSE_GYROSCOPE,
			{
				codeuse : {
					pin : 11,
					radius : 2.4f,
					step_per_round : 800
				}
			}
		},
		moteurs : {
			{4, 9, 6, wheel_radius: 3.0f, GAUCHE},
			{7, 8, 5, wheel_radius: 3.0f, DROITE}
		},
		sicks: {36, 38},
		dureeMatch: 90000000L,
		min_speed: 70,
		max_speed: 255,
		dist : PID(10.f, 0.f, 0.f),
		rot : PID(8.f, 0.f, 0.f)
	};
	Robot.setup(conf);

	//waitTirette;
}

void loop()
{
	parser.loop();

	//loop_actions();
	Robot.loop_pid();
}
