#include "Robot.h"

#include <PWM.h>

Robot::Robot()
{
	InitTimersSafe();
}

Robot::~Robot()
{}


void Robot::setup(Config_Robot _config)
{
    config = _config;

	/* TODO: Calcul chemin */

    setup_moteurs();
	setup_capteurs();
	setup_actionneurs();

    waitTirette();
}

void Robot::setup_moteurs()
{
    Serial << "Setup des moteurs" << endl;

	moteurs[GAUCHE].setup(config.pinMoteurs[GAUCHE]);
	moteurs[DROITE].setup(config.pinMoteurs[DROITE]);
}

void Robot::loop()
{
    if (elapsedTime() > config.dureeMatch)
        arret();

	loop_debug();

	loop_capteurs();
	loop_actionneurs();

	loop_avancer();
	loop_tourner();
}

void Robot::arret()
{
    Serial << "Arret complet du robot" << endl;

    consigneMoteur(0, 0);
    arret_moteurs();
    arret_actionneurs();
   
    while(1); 
}

void Robot::arret_moteurs()
{
    consigneMoteur(0, 0);
    Moteur::stop = true;
}


void Robot::waitTirette()
{
    pinMode(config.pinTirette, INPUT_PULLUP);

    if (digitalRead(config.pinTirette) == HIGH)
        Serial << "En attente de la tirette sur la pin " << config.pinTirette << endl;
    else
    {
        do { delay(500); }
        while (digitalRead(config.pinTirette) == HIGH);
    }

    Serial << "Debut du match!" << endl;
    debutMatch = millis();
}

unsigned long Robot::elapsedTime()
{
    return millis() - debutMatch;
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

    int i;
    for (i = 0; i < numV; i++)
    {
        if (restant >= d[i])
        {
            consigneMoteur(v[i], v[i]);
            break;
        }
        if (restant <= -d[i])
        {
            consigneMoteur(-v[i], -v[i]);
            break;
        }
    }

    if (i == numV)
    {
        consigne_tourner = false;
        consigneMoteur(0, 0);
    }
}

void Robot::loop_debug()
{
    if (config.parser != NULL)
        return;

	while (Serial.available())
	{
		char c = Serial.read();

		if (c == '\n')
		{
			config.parser->parse(cmd.c_str());
            cmd.remove(0); // clear string
		}
		else
			cmd.concat(c);
	}
}


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

void Robot::consigneMoteur(int consigne_gauche, int consigne_droite)
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