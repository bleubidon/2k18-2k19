#include <SimpleTimer.h>

#include "Moteur.h"

#define DEBUG_BUF_SIZE 255

#define GAUCHE 0
#define DROITE 1
#define SEULEMENT 2


struct Config_Robot;
class Robot
{
	public:
		Robot(Config_Robot config);
		virtual ~Robot();

		void setup();
		void loop();

		virtual void setup_capteurs() = 0;
		virtual void setup_actionneurs() = 0;

		virtual void loop_capteurs() = 0;
		virtual void loop_actionneurs() = 0;

		// Deplacement
		virtual float getX() = 0;
		virtual float getY() = 0;
		virtual float getAlpha() = 0;

		void setup_avancer(int distance);
		void setup_tourner(int angle);

		// DEBUG helpers
		void loop_debug();
		virtual void commande_debug(String command, int param);

	private:
		void loop_avancer();
		void loop_tourner();

		void sendConsigneMoteur(int vitesse, float erreur);
		void consigneMoteur(int consigne_vitesse1, int consigne_vitesse2);

		int couleur;
		SimpleTimer timer;

		// Deplacement
		Moteur moteurs[2];

		boolean consigne_avancer = false;
		boolean consigne_tourner = false;

		static const int numV = 4;
		int v[numV] = {150, 150, 150, 90};
		int d[numV] = {250, 150, 10, 2};

		int sens, h, xInitial, yInitial, angleInitial;
		int a;

		// DEBUG helpers
		String fullCommand;
};

struct Config_Robot
{
	int couleur;
	int dureeMatch;

	int pinTirette;
	int pinMoteurs[2][3];
};