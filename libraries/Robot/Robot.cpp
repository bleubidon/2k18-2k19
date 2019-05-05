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

	min_speed = config.min_speed;
	max_speed = config.max_speed;

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

int c_Robot::scale(float speed)
{
	if (speed > 0)
		return min(speed + min_speed, max_speed);
	else
		return max(speed - min_speed, -max_speed);
}

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

	const float precision_dist = 0.5f;
	const float precision_rot = 0.5f;
	if (abs(dist.erreur) < precision_dist && abs(rot.erreur) < precision_rot)
	{
		stop();
		return false;
	}
	else
	{
		moteurs[0].consigne(scale(vitesse_dist + vitesse_rot));
		moteurs[1].consigne(scale(vitesse_dist - vitesse_rot));
	}

	// Ecrit les donnees de log sur le port serie
	LOG(Serial << "PID:" << current_time << "," <<
		dist.consigne << "," << position.dist() << "," <<
		rot.consigne << "," << position.rot() << endl;)
	return true;
}
