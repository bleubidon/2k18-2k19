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

		uint8_t equipe;
		uint8_t pinTirette;
		unsigned long dureeMatch;

		Odometrie position;
		Moteur moteurs[2];

	private:
		// Deplacement
		// TODO: Implementer un vrai asservissement
		void loop_avancer();
		void loop_tourner();

		void sendConsigneMoteurs(int vitesse, float erreur);
		void consigneMoteurs(int consigne_vitesse1, int consigne_vitesse2);


		bool consigne_avancer = false;
		bool consigne_tourner = false;

		static const int numV = 4;
		int v_steps[numV] = {190, 150, 100, 70};
		int d_steps[numV] = {250, 150, 15, 2};

		int accel_max;
		vec startPos;
		float startAngle;
		int sens, d;
		int a;
};

extern c_Robot Robot;
