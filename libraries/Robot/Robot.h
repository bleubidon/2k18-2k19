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
			//unsigned long dureeMatch;

			int min_speed, max_speed;

			PID dist;
			PID rot;
		};

		void setup(c_Robot::Config config);

		// dist: centimetres, rot: degres
		void stop();
		void consigne(float _dist, float _rot);
		void consigne_rel(float _dist, float _rot);

		// helpers
		void translate(float _dist, bool blocking = true);
		void rotate(float _angle, bool blocking = true);
		void go_to(vec _dest, bool blocking = true);
		void look_at(vec _point, bool blocking = true);

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
		//unsigned long duration;

		// PID
		PID dist, rot;
		bool consigne_pid;
		unsigned long prev_time;

		int initial_dist;
};

extern c_Robot Robot;
