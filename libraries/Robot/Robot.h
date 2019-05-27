#pragma once


#include "PID.h"
#include "Sick.h"
#include "Moteur.h"
#include "Odometrie.h"

const int NUM_SICKS = 4;

class c_Robot
{
	public:
		struct Config
		{
			Odometrie::Config odometrie;
			Moteur::Config moteurs[2];
			int sicks[NUM_SICKS];

			int min_speed, max_speed;

			PID dist;
			PID rot;
		};

		void setup(c_Robot::Config config);

		// dist: centimetres, rot: degres
		void stop();
		void consigne(float _dist, float _rot);
		void consigne_rel(float _dist, float _rot);

		int scale(float speed);
		bool loop_pid();

		Odometrie& pos() { return position; }
		PID& dist_pid() { return dist; }
		PID& rot_pid() { return rot; }

		Sick capteurs[NUM_SICKS];

	private:
		Odometrie position;
		Moteur moteurs[2];

		int min_speed, max_speed;

		// PID
		PID dist, rot;
		bool consigne_pid;
		unsigned long prev_time;

		int initial_dist;
};

extern c_Robot Robot;
