#include "Robot.h"

#include <PWM.h>


void Robot::setup(Robot::Config config)
{
	couleur = config.couleur;
	dureeMatch = config.dureeMatch;

	position.setup(config.odometrie);

	InitTimersSafe();
	moteurs[GAUCHE].setup(config.moteurs[GAUCHE]);
	moteurs[DROITE].setup(config.moteurs[DROITE]);

	pinTirette = config.pinTirette;
	
	debutMatch = millis(); // In case waitTirette is not called
}

void Robot::loop()
{
	if (getElapsedTime() > dureeMatch)
		stop();

	loop_avancer();
	loop_tourner();
}

void Robot::stop()
{
	Serial << "Arret complet du robot" << endl;

	consigneMoteurs(0, 0);
	Moteur::stop = true;
   
	while(1); 
}


void Robot::waitTirette()
{
	pinMode(pinTirette, INPUT_PULLUP);

	if (digitalRead(pinTirette) == HIGH)
		Serial << "En attente de la tirette sur la pin " << pinTirette << endl;
	else
	{
		do { delay(500); }
		while (digitalRead(pinTirette) == HIGH);
	}

	Serial << endl << "Debut du match!" << endl;
	debutMatch = millis();
}

unsigned long Robot::getElapsedTime()
{
	return millis() - debutMatch;
}


// Deplacement
void Robot::setup_avancer(int distance)
{
	sens = (distance >= 0);
	h = abs(distance);

	xInitial = position.getX();
	yInitial = position.getY();
	
	angleInitial = position.getAlpha();
	if (angleInitial > 180.0f)
		angleInitial -= 360.0f;

	consigne_avancer = true;
}

void Robot::setup_tourner(int angle)
{
	a = angle;
	if (a > 180.0f)
		a -= 360.0f;

	consigne_tourner = true;
}


// private
int distance(int ax, int ay, int bx, int by);

void Robot::loop_avancer()
{
	if (!consigne_avancer)
		return;

	float erreurAngle = position.getAlpha() - angleInitial;
	if (position.getAlpha() > 180)
		erreurAngle -= 360.0f;


	int L = distance((int)position.getX(), (int)position.getY(), xInitial, yInitial); // distance parcourue
	int restant = h - L;
	
	int i;
	for (i = 0; i < numV; i++)
	{
		if (restant >= d[i])
		{
			sendConsigneMoteurs(v[i], erreurAngle);
			break;
		}
	}

	if (i == numV) // Consider we are close enough
	{
		consigne_avancer = false;
		consigneMoteurs(0, 0);
	}
}

void Robot::loop_tourner()
{
	const float precision = 1.0f; // degré
	
	if (!consigne_tourner)
		return;


	float restant = a - position.getAlpha();
	if (position.getAlpha() > 180)
		restant += 360.0f;

	int i;
	for (i = 0; i < numV; i++)
	{
		if (restant >= d[i])
		{
			consigneMoteurs(v[i], v[i]);
			break;
		}
		if (restant <= -d[i])
		{
			consigneMoteurs(-v[i], -v[i]);
			break;
		}
	}

	if (i == numV)
	{
		consigne_tourner = false;
		consigneMoteurs(0, 0);
	}
}



/// Helpers
int distance(int ax, int ay, int bx, int by)
{
	int x = ax - bx;
	int y = ay - by;

	return sqrt(x*x + y*y);
}

void Robot::sendConsigneMoteurs(int vitesse, float erreur) // sens : 0 pour reculer, 1 pour avancer
{
	int f = 20;
	int vg = vitesse, vd = vitesse;

	if (sens == 0) // reculer
		vg = -vg;
	else		   // avancer
		vd = -vd;
		
		
	if (erreur > 0)
	{
		if (sens == 0)
			consigneMoteurs(vg, vd + f*erreur);
		else
			consigneMoteurs(vg, vd - f*erreur);
	}

	else if (erreur < 0)
	{
		if (sens == 0)
			consigneMoteurs(vg + f*erreur, vd);
		else
			consigneMoteurs(vg - f*erreur, vd);
	}

	else
		consigneMoteurs(vg, vd);
}

void Robot::consigneMoteurs(int consigne_gauche, int consigne_droite)
{
	consigne_gauche = max(-255, min(consigne_gauche, 255));
	consigne_droite = max(-255, min(consigne_droite, 255));

	if (consigne_gauche > 0)
		moteurs[GAUCHE].consigne(CCW, consigne_gauche);
	else
		moteurs[GAUCHE].consigne(CW, -consigne_gauche);

	if (consigne_droite > 0)
		moteurs[DROITE].consigne(CW, consigne_droite);
	else
		moteurs[DROITE].consigne(CCW, -consigne_droite);
} 