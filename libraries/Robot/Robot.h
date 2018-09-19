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
			float P, I, D;
			
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
		void set_consigne(float _consigne_g, float _consigne_d);
		void set_coefs_PID(float P, float I, float D);

		void setup_avancer(int distance);
		void setup_tourner(int angle);

		// Autre deplacement
		int setup_goto(int x, int y, int angle);
		int loop_goto();

		uint8_t equipe;
		uint8_t pinTirette;
		unsigned long dureeMatch;

		Odometrie position;
		Moteur moteurs[2];

	private:
		void asserv();
		void loop_avancer();
		void loop_tourner();

		void sendConsigneMoteurs(int vitesse, float erreur);
		void consigneMoteurs(int consigne_vitesse1, int consigne_vitesse2);

		// Deplacement PID
		float coef_P, coef_I, coef_D;
		float erreur_position, integrale;
		float positions[2], vitesses[2];

		// Deplacement hugues
		bool consigne_avancer = false;
		bool consigne_tourner = false;

		static const int numV = 4;
		int v[numV] = {190, 150, 100, 70};
		int d[numV] = {250, 150, 15, 2};

		int sens, h, xInitial, yInitial, angleInitial;
		int a;

		// Autre deplacement
		int speed;
		int accel_max;
		int prev_distance;
		unsigned long prev_millis;

		// PathFinding
		Path path;
};

extern c_Robot Robot;
