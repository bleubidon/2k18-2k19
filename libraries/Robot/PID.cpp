#include "PID.h"

PID::PID(float _Kp, float _Ki, float _Kd)
{
	Kp = _Kp;
	Ki = _Ki;
	Kd = _Kd;
}

void PID::set_consigne(float _consigne)
{
	consigne = _consigne;
	erreur = 0.0f;
	vitesse = 0.0f;
	integrale = 0.0f;
}

void PID::set_coefs(float _Kp, float _Ki, float _Kd)
{
	Kp = _Kp;
	Ki = _Ki;
	Kd = _Kd;
}

float PID::compute(float mesure, float dt)
{
	float vitesse_old = vitesse;
	float erreur_old = erreur;

	erreur = consigne - mesure;
	integrale += erreur * dt / 1000;
	float derivee = (erreur - erreur_old) * 1000 / dt;

	vitesse	= Kp * erreur
			+ Ki * integrale
			+ Kd * derivee;

	return vitesse;
}
