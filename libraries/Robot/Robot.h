#pragma once

#include <Action.h>
#include <Moteur.h>
#include <Odometrie.h>

#define GAUCHE 0
#define DROITE 1


class Robot
{
	public:
		struct Config
		{
			int couleur;
			unsigned long dureeMatch;

			Odometrie::Config odometrie;
			Moteur::Config moteurs[2];
			int pinTirette;
		};


		void setup(Robot::Config config);
		void loop();
		void stop();

		void waitTirette();
		unsigned long getElapsedTime();

		// Deplacement
		void setup_avancer(int distance);
		void setup_tourner(int angle);


		int couleur;
		unsigned long dureeMatch;

		Odometrie position;
		Moteur moteurs[2];
		int pinTirette;

	private:
		void loop_avancer();
		void loop_tourner();

		void sendConsigneMoteurs(int vitesse, float erreur);
		void consigneMoteurs(int consigne_vitesse1, int consigne_vitesse2);

		unsigned long debutMatch;

		// Deplacement
		boolean consigne_avancer = false;
		boolean consigne_tourner = false;

		static const int numV = 4;
		int v[numV] = {150, 110, 80, 30};
		int d[numV] = {250, 150, 10, 2};

		int sens, h, xInitial, yInitial, angleInitial;
		int a;
};


// Serial print helpers
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
const char endl = '\n';