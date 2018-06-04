#include <SimpleTimer.h>


#define DEBUG_BUF_SIZE 255

#define GAUCHE 0
#define DROITE 1
#define SEULEMENT 2


class Robot
{
	public:
		Robot(int _couleur);
		virtual ~Robot();

		void definirChemin(int _couleur, WayPoint[] chemin);
		void loop();

		virtual void setup_actionneurs() = 0;
		virtual void loop_actionneurs() = 0;

		// Deplacement
		void setup_avancer(int distance);
		void setup_tourner(int angle);

		// DEBUG helpers
		void debug_loop();
		virtual void debug_commande(char* command, int param);

	private:
		void loop_avancer();
		void loop_tourner();


		int couleur;
		SimpleTimer timer;

		// Deplacement
		const int numV = 4;

		int vd[numV] = {150, 150, 150, 90};
		int vg[numV] = {150, 150, 150, 90};

		int d[numV] = {250, 150, 10, 2};

		int sens, h, xInitial, yInitial, angleInitial;
		int a;

		// DEBUG helpers
		char* buffer[DEBUG_BUF_SIZE];
		int cursor;
}