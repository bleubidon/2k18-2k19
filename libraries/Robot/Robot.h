#pragma once

#include "PID.h"
#include "Moteur.h"
#include "Odometrie.h"
#include "TaskQueue.h"

class c_Robot
{
	public:
		struct Config
		{
			Odometrie::Config odometrie;
			Moteur::Config moteurs[2];

			PID dist;
			PID rot;
		};

		void setup(c_Robot::Config config);

		void stop();
		void consigne(float _dist, float _rot);
		void consigne_rel(float _dist, float _rot);

		bool loop_pid();

		Odometrie& pos() { return position; }
		PID& dist_pid() { return dist; }
		PID& rot_pid() { return rot; }

	//private:
		Odometrie position;
		Moteur moteurs[2];

		// PID
		PID dist, rot;
		bool consigne_pid;
		unsigned long prev_time;

		int initial_dist;
};

extern c_Robot Robot;
