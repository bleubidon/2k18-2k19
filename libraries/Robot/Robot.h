#pragma once

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
			int accel_max;
		};

		void setup(c_Robot::Config config);
		void stop();

		// Deplacement
		void setup_avancer(int distance);
		void setup_tourner(int angle);
		void setup_pid();

		uint8_t equipe;
		uint8_t pinTirette;
		unsigned long dureeMatch;

		Odometrie position;
		Moteur moteurs[2];

	//private:
		// Deplacement
		// TODO: Implementer un vrai asservissement
		void loop_avancer();
		void loop_tourner();
		void loop_pid();

		void sendConsigneMoteurs(int vitesse, float erreur);
		void consigneMoteurs(int consigne_vitesse1, int consigne_vitesse2);


		bool consigne_avancer = false;
		bool consigne_tourner = false;
		bool consigne_pid = false;

		static const int numV = 4;
		int v_steps[numV] = {130, 110, 90, 30};
		int d_steps[numV] = {250, 150, 15, 2};

		int accel_max;
		vec startPos;
		float startAngle;
		int sens, d;
		int a;

		// PID
		float coef_P = 1.0f, coef_I = 0.0f, coef_D = 0.0f;

		unsigned long prev_time;
		float erreur_position[2], integrale[2];
		float positions[2], vitesses[2];
};

extern c_Robot Robot;
