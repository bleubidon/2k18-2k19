#pragma once


#include "PID.h"
#include "Sick.h"
#include "Moteur.h"
#include "Odometrie.h"


class c_Robot
{
	public:
		struct Config
		{
			Odometrie::Config odometrie;
			Moteur::Config moteurs[2];
			int num_sicks, *sicks;
			unsigned long dureeMatch;

			int min_speed, max_speed;

			PID dist;
			PID rot;
		};

		void setup(c_Robot::Config config);

		void start();
		void stop();

		// dist: centimetres, rot: degres
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


		Odometrie position;
		Moteur moteurs[2];

		int num_sicks;
		Sick *capteurs;

		unsigned long start_time, duration;
		int min_speed, max_speed;

		// PID
		PID dist, rot;
		bool consigne_pid;
		unsigned long prev_time;

		int initial_dist;
};

extern c_Robot Robot;
