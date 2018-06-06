#pragma once

#include <Parser.h>
#include <Moteur.h>

#define GAUCHE 0
#define DROITE 1


struct Config_Robot
{
	int couleur;
	unsigned long dureeMatch;

	int pinTirette;
	int pinMoteurs[2][3];

	Parser* parser;
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

	//protected:
		Config_Robot config;

	//private:
		void loop_debug();

		void loop_avancer();
		void loop_tourner();

		void sendConsigneMoteurs(int vitesse, float erreur);
		void consigneMoteurs(int consigne_vitesse1, int consigne_vitesse2);

		unsigned long debutMatch;
		String cmd;

		Moteur moteurs[2];

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