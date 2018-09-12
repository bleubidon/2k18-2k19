#include "Robot.h"

#include <PWM.h>

const uint8_t GAUCHE = 0;
const uint8_t DROITE = 1;
const char endl = '\n';

c_Robot Robot;

void c_Robot::setup(c_Robot::Config config)
{
	// init PID
	coef_P = config.P;
	coef_I = config.I;
	coef_D = config.D;
	
	erreur_position = integrale = 0.0f;
	positions[0] = positions[1] = 0.0f;

	couleur = config.couleur;
	dureeMatch = config.dureeMatch;

	Serial << "Setup position..." << endl;
	position.setup(config.odometrie);

	Serial << "Setup moteurs..." << endl;
	InitTimersSafe();
	moteurs[GAUCHE].setup(config.moteurs[GAUCHE]);
	moteurs[DROITE].setup(config.moteurs[DROITE]);

	accel_max = config.accel_max;
	pinTirette = config.pinTirette;

	debutMatch = millis(); // In case waitTirette is not called
}

void c_Robot::loop()
{
	if (getElapsedTime() > dureeMatch)
		stop();

	position.update();
}

void c_Robot::stop()
{
	Serial << "Arret complet du robot" << endl;

	consigneMoteurs(0, 0);
	Moteur::stop = true;

	while (1)
		;
}

void c_Robot::waitTirette()
{
	pinMode(pinTirette, INPUT_PULLUP);

	while (digitalRead(pinTirette) == LOW)
	{
		Serial << "En attente de la tirette sur la pin " << pinTirette << endl;
		delay(1000);
	}

	debutMatch = millis();
}

unsigned long c_Robot::getElapsedTime()
{
	return millis() - debutMatch;
}

// Deplacement
void c_Robot::set_consigne(float _consigne_g, float _consigne_d)
{
	positions[GAUCHE] = _consigne_g;
	positions[DROITE] = _consigne_d;
}

void c_Robot::set_coefs_PID(float P, float I, float D)
{
	Serial << "new coefs: " << P << " " << I << " " << D << endl;

	coef_P = P;
	coef_I = I;
	coef_D = D;
}

void c_Robot::setup_avancer(int distance)
{
	sens = (distance >= 0);
	h = abs(distance);

	xInitial = position.getX();
	yInitial = position.getY();

	Serial << "xi: " << xInitial << "\tyi: " << yInitial << endl;

	angleInitial = position.getAlpha();
	if (angleInitial > 180.0f)
		angleInitial -= 360.0f;

	consigne_avancer = true;
}

void c_Robot::setup_tourner(int angle)
{
	a = angle;
	if (a > 180.0f)
		a -= 360.0f;

	consigne_tourner = true;
}

// Autre deplacement
int c_Robot::setup_goto(int x, int y, int angle)
{
	// do the pathfing
	vec pos(position.getX(), position.getY());
	prev_distance = path.find(pos);
	prev_millis = millis();
	speed = 0;
	return prev_distance;
}

Print &operator<<(Print &obj, vec p)
{
	obj.print('(');
	obj.print(p.x);
	obj.print(", ");
	obj.print(p.y);
	obj.print(')');
	return obj;
}

int c_Robot::loop_goto()
{
	vec current_pos(position.getX(), position.getY());
	const int distance = path.get_distance(current_pos);
	if (distance < 2)
	{
		moteurs[GAUCHE].consigne(0);
		moteurs[DROITE].consigne(0);
		return false;
	}
	else
	{
		const unsigned long curr = millis();
		int speed_goal = 220;
		//if (speed*speed / (2 * accel_max) < distance)
		//	speed_goal = 0;

		int dvMax = accel_max * (curr - prev_millis) / 100;
		int dv = clamp(-dvMax, (speed_goal - speed), dvMax);
		speed += dv;

		unsigned long ed = speed;
		ed = ed * ed / (2 * accel_max);
		Serial << "ed: " << ed << " ";

		prev_millis = curr;
		prev_distance = distance;
	}

	vec current_dir(position.dirX, position.dirY);
	vec perpendicular(current_dir.y, -current_dir.x);
	vec goal_dir = path.get_direction(current_pos, current_dir);

	float proj = dot(perpendicular, goal_dir);
	Serial << "dist: " << distance << " goal: " << goal_dir << " curr: " << current_dir << " pos: " << current_pos;

	if (dot(current_dir, goal_dir) > 0)
	{
		float p = (proj + 1) / 2;
		if (proj < 0) // go left
		{
			moteurs[GAUCHE].consigne(speed);
			moteurs[DROITE].consigne(speed * p / (1 - p));
		}
		else
		{
			moteurs[GAUCHE].consigne(speed * (1 - p) / p);
			moteurs[DROITE].consigne(speed);
		}
		Serial << "   speed: " << speed;
		//float rpm = 60 * speed / (3.14 * wheel_radius);
	}
	else //demi tour
	{
		int real_speed = speed;
		if (proj < 0)
			real_speed *= -1;
		moteurs[GAUCHE].consigne(-real_speed);
		moteurs[DROITE].consigne(real_speed);
		//Serial << "   demi-tour";
	}

	Serial << endl;
	return true;
}

// private
void c_Robot::asserv()
{
	static const float vMax = 100;
	static const float aMax = 100;

	for (int i = 0; i < 2; i++)
	{
		// filtre PID
		float vitesse_old = vitesses[i];
		float erreur_position_old = erreur_position;

		erreur_position = positions[i] - position.getPositionCodeuse(i);
		integrale += erreur_position;
		float derivee = erreur_position - erreur_position_old;

		vitesses[i] = coef_P * erreur_position +
					  coef_I * integrale +
					  coef_D * derivee;

		// Écrêtages
		vitesses[i] = clamp(-vMax, vitesses[i], vMax);
		vitesses[i] = clamp(-aMax, vitesses[i] - vitesse_old, aMax)  + vitesse_old;
	}

	consigneMoteurs(vitesses[0], vitesses[1]);
}

int distance(int ax, int ay, int bx, int by);

void c_Robot::loop_avancer()
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
		Serial << "xf: " << position.getX() << "\tyf: " << position.getY() << endl;
		consigne_avancer = false;
		consigneMoteurs(0, 0);
	}
}

void c_Robot::loop_tourner()
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

	return sqrt(x * x + y * y);
}

void c_Robot::sendConsigneMoteurs(int vitesse, float erreur) // sens : 0 pour reculer, 1 pour avancer
{
	int f = 20;
	int vg = vitesse, vd = vitesse;

	if (sens == 0) // reculer
		vg = -vg;
	else // avancer
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
