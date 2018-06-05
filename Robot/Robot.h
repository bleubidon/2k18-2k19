#pragma once

#include <Moteur.h>

#define DEBUG_BUF_SIZE 255

#define GAUCHE 0
#define DROITE 1
#define SEULEMENT 2


struct Config_Robot
{
	int couleur;
	unsigned long dureeMatch;

	int pinTirette;
	int pinMoteurs[2][3];
};

class Robot
{
	public:
		Robot();
		virtual ~Robot();

		virtual void setup(Config_Robot _config);
				void setup_moteurs();
		virtual void setup_capteurs() = 0;
		virtual void setup_actionneurs() = 0;

		virtual void loop();
			 // void loop_moteurs(); // pour l'asserv ?
		virtual void loop_capteurs() = 0;
		virtual void loop_actionneurs() = 0;

		virtual void arret();
				void arret_moteurs();
		virtual void arret_actionneurs() = 0;


		void waitTirette();
		unsigned long elapsedTime();

		// Deplacement
		virtual float getX() = 0;
		virtual float getY() = 0;
		virtual float getAlpha() = 0;

		void setup_avancer(int distance);
		void setup_tourner(int angle);

		virtual void commande_debug(String command, int param);

		Config_Robot config;
		unsigned long debutMatch;


	private:
		void loop_avancer();
		void loop_tourner();

		void sendConsigneMoteur(int vitesse, float erreur);
		void consigneMoteur(int consigne_vitesse1, int consigne_vitesse2);

		Moteur moteurs[2];

		boolean consigne_avancer = false;
		boolean consigne_tourner = false;

		static const int numV = 4;
		int v[numV] = {150, 150, 150, 90};
		int d[numV] = {250, 150, 10, 2};

		int sens, h, xInitial, yInitial, angleInitial;
		int a;

		// DEBUG helpers
		void loop_debug();
		String fullCommand;
};


// Serial print helpers
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
const char endl = '\n';