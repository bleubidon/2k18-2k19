#pragma once

#include <Moteur.h>
#include <Odometrie.h>
#include <TaskQueue.h>

class Robot
{
	public:
		struct Config
		{
			float P, I, D;
			
			uint8_t couleur;
			unsigned long dureeMatch;

			Odometrie::Config odometrie;
			Moteur::Config moteurs[2];
			uint8_t pinTirette;
		};


		void setup(Robot::Config config);
		void loop();
		void stop();

		void waitTirette();
		unsigned long getElapsedTime();

		// Deplacement
		void set_consigne(float _consigne_g, float _consigne_d);
		void set_coefs_PID(float P, float I, float D);

		void setup_avancer(int distance);
		void setup_tourner(int angle);


		uint8_t couleur;
		unsigned long dureeMatch;

		Odometrie position;
		Moteur moteurs[2];
		uint8_t pinTirette;

	private:
		void asserv();
		void loop_avancer();
		void loop_tourner();

		void sendConsigneMoteurs(int vitesse, float erreur);
		void consigneMoteurs(int consigne_vitesse1, int consigne_vitesse2);

		unsigned long debutMatch;

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
};


inline int clamp(int _min, int _val, int _max)
{
	return max(_min, min(_val, _max));
}

// Serial print helpers
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
const char endl = '\n';
