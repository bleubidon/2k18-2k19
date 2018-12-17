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
			uint8_t equipe;
			uint8_t pinTirette;
			unsigned long dureeMatch;

			Odometrie::Config odometrie;
			Moteur::Config moteurs[2];

			PID dist;
			PID rot;
		};

		void setup(c_Robot::Config config);
		void set_pid(float _Kp, float _Ki, float _Kd);

		void stop();
		void consigne(float _dist, float _rot);


		/// USELESS
		// Deplacement hugues
		void setup_avancer(int distance);
		void setup_tourner(int angle);

	//private:
		uint8_t equipe;
		uint8_t pinTirette;
		unsigned long dureeMatch;

		Odometrie position;
		Moteur moteurs[2];

		// PID
		PID dist, rot;
		bool consigne_pid;
		unsigned long prev_time;


		/// USELESS
		// Deplacement hugues
		void loop_avancer();
		void loop_tourner();
		void loop_pid();

		void sendConsigneMoteurs(int vitesse, float erreur);
		void consigneMoteurs(int consigne_vitesse1, int consigne_vitesse2);


		bool consigne_avancer = false;
		bool consigne_tourner = false;

		static const int numV = 4;
		int v_steps[numV] = {130, 110, 90, 30};
		int d_steps[numV] = {250, 150, 15, 2};

		vec startPos;
		float startAngle;
		int sens, d;
		int a;
};

extern c_Robot Robot;
