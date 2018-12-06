#include "Robot.h"

#include <PWM.h>

const uint8_t GAUCHE = 0;
const uint8_t DROITE = 1;
const char endl = '\n';

c_Robot Robot;

void c_Robot::setup(c_Robot::Config config)
{
	equipe = config.equipe;
	pinTirette = config.pinTirette;
	dureeMatch = config.dureeMatch;

	Serial << "Setup position..." << endl;
	position.setup(config.odometrie);

	Serial << "Setup moteurs..." << endl;
	InitTimersSafe();
	moteurs[GAUCHE].setup(config.moteurs[GAUCHE]);
	moteurs[DROITE].setup(config.moteurs[DROITE]);

	accel_max = config.accel_max;

	erreur_position = integrale = 0.0f;
	positions[0] = positions[1] = 0.0f;
	vitesses[0] = vitesses[1] = 0.0f;
}

void c_Robot::stop()
{
	moteurs[GAUCHE].consigne(0);
	moteurs[DROITE].consigne(0);
}

Print &operator<<(Print &obj, vec p)
{
	obj.print('[');
	obj.print(p.x);
	obj.print(", ");
	obj.print(p.y);
	obj.print(']');
	return obj;
}


// Deplacement
void c_Robot::setup_avancer(int distance)
{
	sens = (distance >= 0);
	d = abs(distance);

	position.update();
	startPos = position.pos();
	startAngle = position.rot();

	if (startAngle > 180.0f)
		startAngle -= 360.0f;

	consigne_avancer = true;

	DEBUG(Serial << "Position initiale: " << startPos << endl);
	DEBUG(Serial << "Angle initial: " << startAngle << endl);
}

void c_Robot::setup_tourner(int angle)
{
	a = angle;
	if (a > 180.0f)
		a -= 360.0f;

	consigne_tourner = true;
}

void c_Robot::loop_pid()
{
	position.update();

	float coef_P = 1.0f, coef_I = 0.0f, coef_D = 0.0f;

	float consignes[2] = {-20.0f, 20.0f};


	for (int i = 0; i < 2; i++)
	{
		// filtre PID
		float vitesse_old = vitesses[i];
		float erreur_position_old = erreur_position;

		erreur_position = consignes[i] - position.getPositionCodeuse(i);
		integrale += erreur_position;
		float derivee = erreur_position - erreur_position_old;

		vitesses[i] = coef_P * erreur_position +
					  coef_I * integrale +
					  coef_D * derivee;

		// Écrêtages
		//vitesses[i] = clamp(-vMax, vitesses[i], vMax);
		//vitesses[i] = clamp(-aMax, vitesses[i] - vitesse_old, aMax)  + vitesse_old;

		moteurs[i].consigne(vitesses[i]);
	}
}

void c_Robot::loop_avancer()
{
	if (!consigne_avancer)
		return;

	position.update();
	float erreurAngle = position.rot() - startAngle;
	if (position.rot() > 180.0f)
		erreurAngle -= 360.0f;

	int covered = dist(position.pos(), startPos);
	int remaining = d - covered;

	int i;
	for (i = 0; i < numV; i++)
	{
		if (remaining >= d_steps[i])
		{
			sendConsigneMoteurs(v_steps[i], erreurAngle);
			break;
		}
	}

	if (i == numV) // We consider we are close enough
	{
		stop();
		consigne_avancer = false;
		DEBUG(Serial << "Position finale: " << position.pos() << endl);
	}
}

void c_Robot::loop_tourner()
{
	if (!consigne_tourner)
		return;

	position.update();
	int remaining = a - (int)position.rot();
	if (position.rot() > 180)
		remaining += 360.0f;

	int i;
	for (i = 0; i < numV; i++)
	{
		if (remaining >= d_steps[i])
		{
			consigneMoteurs(v_steps[i], v_steps[i]);
			break;
		}
		if (remaining <= -d_steps[i])
		{
			consigneMoteurs(-v_steps[i], -v_steps[i]);
			break;
		}
	}

	if (i == numV)
	{
		stop();
		consigne_tourner = false;
	}
}

/// Helpers
void c_Robot::sendConsigneMoteurs(int vitesse, float erreur)
{
	int f = 20;
	int vg = vitesse, vd = vitesse;

	// sens : 0 pour reculer, 1 pour avancer
	if (sens == 0)
		vg = -vg;
	else
		vd = -vd;

	if (erreur > 0)
	{
		if (sens == 0)
			consigneMoteurs(vg, vd + f * erreur);
		else
			consigneMoteurs(vg, vd - f * erreur);
	}

	else if (erreur < 0)
	{
		if (sens == 0)
			consigneMoteurs(vg + f * erreur, vd);
		else
			consigneMoteurs(vg - f * erreur, vd);
	}

	else
		consigneMoteurs(vg, vd);
}

void c_Robot::consigneMoteurs(int consigne_gauche, int consigne_droite)
{
	consigne_gauche = clamp(-255, consigne_gauche, 255);
	consigne_droite = clamp(-255, consigne_droite, 255);

	if (consigne_gauche > 0)
		moteurs[GAUCHE].consigne(CCW, consigne_gauche);
	else
		moteurs[GAUCHE].consigne(CW, -consigne_gauche);

	if (consigne_droite > 0)
		moteurs[DROITE].consigne(CW, consigne_droite);
	else
		moteurs[DROITE].consigne(CCW, -consigne_droite);

	/* Sould be equivalent to the following */
	/*
	moteurs[GAUCHE].consigne(consigne_gauche);
	moteurs[DROITE].consigne(consigne_droite);
	*/
}