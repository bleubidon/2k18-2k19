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

void c_Robot::setup_pid()
{
	erreur_position[0] = erreur_position[1] = 0.0f;
	integrale[0] = integrale[1] = 0.0f;
	positions[0] = positions[1] = 0.0f;
	vitesses[0] = vitesses[1] = 0.0f;

	prev_time = millis();
	consigne_pid = true;
}

void c_Robot::loop_pid()
{
	position.update();

	const unsigned long min_delay = 10;
	const float precision_seuil = 1;
	const float vMax = 30;
	const float aMax = 80;

	float consignes[2] = {60.0f, 60.0f};

	unsigned long current_time = millis();
	unsigned long dt = current_time - prev_time;
	if (dt < min_delay || !consigne_pid)
		return;
	
	float derivee[2];
	for (int i = 0; i < 2; i++)
	{
		// filtre PID
		float vitesse_old = vitesses[i];
		float erreur_position_old = erreur_position[i];

		erreur_position[i] = consignes[i] - position.getPositionCodeuse(i);
		integrale[i] += erreur_position[i] * dt / 1000;
		derivee[i] = (erreur_position[i] - erreur_position_old) * 1000 / dt;

		vitesses[i] =	coef_P * erreur_position[i] +
				coef_I * integrale[i] +
				coef_D * derivee[i];

		// Écrêtages
		vitesses[i] = clamp(-vMax, vitesses[i], vMax);
		//vitesses[i] = clamp(-aMax, (vitesses[i] - vitesse_old) / dt, aMax)  + vitesse_old;
//DEBUG(Serial << "vitesse[" << i << "] = " << vitesses[i] << endl;);

		moteurs[i].consigne(vitesses[i]);
	}
DEBUG(Serial << current_time << "," << vitesses[0] << "," << vitesses[1]  << "," << erreur_position[0] << "," << erreur_position[1] << "," << integrale[0] << "," << integrale[1] << "," << derivee[0] << "," << derivee[1] << "\n" ;);
	
	if (abs(erreur_position[0]) < precision_seuil && abs(erreur_position[1]) < precision_seuil)
	{
		stop();
		consigne_pid = false;
DEBUG(Serial << "Destination reached" << endl;);
	}

	prev_time = current_time;
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