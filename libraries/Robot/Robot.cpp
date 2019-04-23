#include "Robot.h"

#include <PWM.h>

const uint8_t GAUCHE = 0;
const uint8_t DROITE = 1;
const char endl = '\n';

c_Robot Robot;

void c_Robot::setup(c_Robot::Config config)
{
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

	consigne_pid = false;
}

void c_Robot::consigne(float _dist, float _rot)
{
	dist.set_consigne(_dist);
	rot.set_consigne(_rot);

	prev_time = millis();
	consigne_pid = true;

	LOG(Serial << "CMD: " << _dist << "," << _rot << endl);
}

void c_Robot::consigne_rel(float _dist, float _rot)
{
	consigne(position.dist() + _dist, position.rot() + _rot);
}

float angle_diff(float a, float b)
{
	if (b < a)
		return -angle_diff(b, a);
	if (b < a + 180)
		return b - a;
	else
		return (b - 360) - a;
}

/* TODO:
** scale down both speeds instead of clamping
*/
bool c_Robot::loop_pid()
{
	if (!consigne_pid)
		return false;

	// Mise a jour de la consigne toute les 10 millisecondes (au moins)
	const unsigned long min_delay = 10;
	unsigned long current_time = millis();
	unsigned long dt = current_time - prev_time;
	if (dt < min_delay)
		return true;
	prev_time = current_time;


	// Lecture des capteurs de position et calcul de la consigne grace aux PIDs
	position.update();

	float vitesse_dist = dist.compute(dist.consigne - position.dist(), dt);
	float vitesse_rot = rot.compute(angle_diff(rot.consigne, position.rot()), dt);

	const float precision_dist = 0.1;
	const float precision_rot = 0.1;
	if (abs(dist.erreur) < precision_dist && abs(rot.erreur) < precision_rot)
	{
		stop();
		return false;
	}
	else
	{
		// Ecretage des vitesses
		const float vMax = 128; //64;
		vitesse_dist = clamp(-vMax, vitesse_dist, vMax);
		vitesse_rot = clamp(-vMax, vitesse_rot, vMax);

		// Ecretage des accelerations
		const float dvMax = 20;
		//vitesse_dist = clamp(-dvMax, (vitesse_dist - dist.vitesse_old), dvMax)  + dist.vitesse_old;
		//vitesse_rot = clamp(-dvMax, (vitesse_rot - rot.vitesse_old), dvMax)  + rot.vitesse_old;

		moteurs[0].consigne(vitesse_dist + vitesse_rot);
		moteurs[1].consigne(vitesse_dist - vitesse_rot);
	}

	// Ecrit les donnees de log sur le port serie
	LOG(Serial << "PID:" << current_time << "," <<
		dist.consigne << "," << position.dist() << "," <<
		rot.consigne << "," << position.rot() << endl;)
	return true;
}
