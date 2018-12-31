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

	position.setup(config.odometrie);

	InitTimersSafe();
	moteurs[GAUCHE].setup(config.moteurs[GAUCHE]);
	moteurs[DROITE].setup(config.moteurs[DROITE]);

	dist = config.dist;
	rot = config.rot;
	consigne_pid = false;
}

void c_Robot::stop()
{
	moteurs[GAUCHE].consigne(0);
	moteurs[DROITE].consigne(0);

	consigne_avancer = consigne_tourner = consigne_pid = false;
}

void c_Robot::consigne(float _dist, float _rot)
{
	dist.set_consigne(_dist);
	rot.set_consigne(_rot);

	prev_time = millis();
	consigne_pid = true;

DEBUG(Serial << "consigne: " << _dist << " " << _rot << endl;)
}

float	angle_error(float a, float b)
{
	float err = a - b;
	if (err > 180)
		err = -180 + (int)err % 180;
	else if (err <= -180)
		err = 180 + (int)err % 180;
	return err;
}

/* TODO:
** scale down both speeds instead of clamping
*/
void c_Robot::loop_pid()
{
	if (!consigne_pid)
		return;

	// Mise a jour de la consigne toute les min_delay millisecondes (au moins)
	const unsigned long min_delay = 10;
	unsigned long current_time = millis();
	unsigned long dt = current_time - prev_time;
	if (dt < min_delay)
		return;
	prev_time = current_time;


	// Lecture des capteurs de position et calcul de la consigne grace aux PIDs
	position.update();

	float vitesse_dist = dist.compute(dist.consigne - position.dist(), dt);
	float vitesse_rot = rot.compute(angle_error(rot.consigne, position.rot()), dt);

	const float precision_dist = 1;
	const float precision_rot = 1;
	if (abs(dist.erreur) < precision_dist && abs(rot.erreur) < precision_rot)
		stop();
	else
	{
		// Ecretage des vitesses
		const float vMax = 20;
		vitesse_dist = clamp(-vMax, vitesse_dist, vMax);
		vitesse_rot = clamp(-vMax, vitesse_rot, vMax);

		// Ecretage des accelerations
		const float dvMax = 20;
		//vitesse_dist = clamp(-dvMax, (vitesse_dist - dist.vitesse_old), dvMax)  + dist.vitesse_old;
		//vitesse_rot = clamp(-dvMax, (vitesse_rot - rot.vitesse_old), dvMax)  + rot.vitesse_old;

		moteurs[0].consigne(vitesse_dist + vitesse_rot);
		moteurs[1].consigne(vitesse_dist - vitesse_rot);
	}

	// Ecrit les donnes sur le port serie pour le debug
DEBUG(Serial << current_time << "," << dist.consigne << "," << position.dist() << "," << rot.consigne << "," << position.rot() << endl;)
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

void c_Robot::loop_avancer()
{
	if (!consigne_avancer)
		return;

	position.update();
	float erreurAngle = position.rot() - startAngle;
	if (position.rot() > 180.0f)
		erreurAngle -= 360.0f;

	int covered = vec::dist(position.pos(), startPos);
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
