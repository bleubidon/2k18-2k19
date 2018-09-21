#pragma once

#include "Moteur.h"
#include "Odometrie.h"
#include "TaskQueue.h"
#include "PathFinding.h"

class c_Robot
{
	public:
		struct Config
		{
			uint8_t equipe;
			uint8_t pinTirette;
			unsigned long dureeMatch;

			Odometrie::Config odometrie;
			Moteur::Config moteurs[2];
			int accel_max;
		};

		void setup(c_Robot::Config config);
		void stop();

		// Deplacement
		int setup_goto(int x, int y, int angle);
		int loop_goto();

		uint8_t equipe;
		uint8_t pinTirette;
		unsigned long dureeMatch;

		Odometrie position;
		Moteur moteurs[2];

	private:
		// Deplacement
		int speed;
		int accel_max;
		int prev_distance;
		unsigned long prev_millis;

		// PathFinding
		Path path;
};

extern c_Robot Robot;
