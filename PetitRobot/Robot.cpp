#include "Robot.h"

#include <PWM.h>

Robot::Robot(Config_Robot config):
    moteurs{ Moteur(config.pinMoteurs[0]), Moteur(config.pinMoteurs[1]) }
{
	InitTimersSafe();
	moteurs[GAUCHE].setup();
	moteurs[DROITE].setup();
}

Robot::~Robot()
{}


void Robot::setup()
{
	/* TODO: Calcul chemin */

	setup_capteurs();
	setup_actionneurs();
}

void Robot::loop()
{
	loop_debug();

	loop_capteurs();

	loop_avancer();
	loop_tourner();
	loop_actionneurs();
}

// Deplacement
int calcDistDiagonal(int posx, int posy, int posxinit, int posyinit);

void Robot::setup_avancer(int distance)
{
    sens = (distance >= 0);
    h = abs(distance);

    xInitial = getX();
    yInitial = getY();
    
    angleInitial = getAlpha();
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


void Robot::loop_avancer()
{
    if (!consigne_avancer)
        return;

    float erreurAngle = getAlpha() - angleInitial;
    if (getAlpha() > 180)
        erreurAngle -= 360.0f;


    int L = calcDistDiagonal(getX(), getY(), xInitial, yInitial); // parcourue
    int restant = h - L;
    
    int i;
    for (i = 0; i < numV; i++)
    {
        if (restant >= d[i])
        {
            sendConsigneMoteur(v[i], erreurAngle);
            break;
        }
    }

    if (i == numV)
    {
        consigne_avancer = false;
        consigneMoteur(0, 0);
    }
}

void Robot::loop_tourner()
{
    const float precision = 1.0f; // degré
    
    if (!consigne_tourner)
        return;


    float restant = a - getAlpha();
    if (getAlpha() > 180)
        restant += 360.0f;

    int vitesse = 1;

    if (abs(restant) < 20.0f)
        vitesse = 3;

    if (restant > precision)
        consigneMoteur(v[vitesse], v[vitesse]);

    else if (restant < -precision)
        consigneMoteur(-v[vitesse], -v[vitesse]);

    else
    {
        consigne_tourner = false;
        consigneMoteur(0, 0);
    }
}

// DEBUG
void Robot::loop_debug()
{
	while (Serial.available())
	{
		char c = Serial.read();

		if (c == '\n')
		{
			int splitPos = fullCommand.indexOf(' ');

			String command = fullCommand.substring(0, splitPos+1);
			int param = fullCommand.substring(splitPos).toInt();

			commande_debug(command, param);
		}
		else
			fullCommand.concat(c);
	}
}

void Robot::commande_debug(String command, int param)
{ }


/// Helpers
int calcDistDiagonal(int posx, int posy, int posxinit, int posyinit)
{
    long x = posx - posxinit;
    long y = posy - posyinit;

    return sqrt(x*x + y*y);
}

void Robot::sendConsigneMoteur(int vitesse, float erreur) // sens : 0 pour reculer, 1 pour avancer
{
    int f = 20;
	int vg = vitesse, vd = vitesse;

    if (sens == 0) // reculer
        vg = -vg;
    else           // avancer
        vd = -vd;
        
        
    if (erreur > 0)
    {
        if (sens == 0)
            consigneMoteur(vg, vd + f*erreur);
        else
            consigneMoteur(vg, vd - f*erreur);
    }

    else if (erreur < 0)
    {
        if (sens == 0)
            consigneMoteur(vg + f*erreur, vd);
        else
            consigneMoteur(vg - f*erreur, vd);
    }

    else
        consigneMoteur(vg, vd);
}

void Robot::consigneMoteur(int consigne_vitesse1, int consigne_vitesse2)
{
	consigne_vitesse1 = max(-255, min(consigne_vitesse1, 255));
	consigne_vitesse2 = max(-255, min(consigne_vitesse2, 255));

	if (consigne_vitesse1 > 0)
		moteurs[0].consigne(CCW, consigne_vitesse1);
	else
		moteurs[0].consigne(CW, -consigne_vitesse1);

	if (consigne_vitesse2 > 0)
		moteurs[1].consigne(CW, consigne_vitesse2);
	else
		moteurs[1].consigne(CCW, -consigne_vitesse2);
} 