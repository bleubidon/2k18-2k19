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

// Deplacement
int c_Robot::setup_goto(int x, int y, int angle)
{
	position.update();
	
	// do the pathfing
	prev_distance = path.find(position.pos());
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
	position.update();

	const int distance = path.get_distance(position.pos());
	if (distance < 2)
	{
		stop();
		return false;
		/// -----------------///
		/*int erreur_angle = position.rot() - a;
		if (position.rot() > 180)
			erreur_angle -= 360;
		int sign = (erreur_angle >= 0) ? 1 : -1;
DEBUG(Serial << "r: " << position.rot() << " a: " << a << " err: " << erreur_angle << " sign: " << sign << endl);
		if (erreur_angle == 0)
		{
			moteurs[GAUCHE].consigne(0);
			moteurs[DROITE].consigne(0);
			return false;
		}
		speed = 100 + min(sign * erreur_angle, 10) * 5;
		moteurs[GAUCHE].consigne(-sign * speed);
		moteurs[DROITE].consigne(sign * speed);
		return true;*/
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
DEBUG(Serial << "ed: " << ed << " ");

		prev_millis = curr;
		prev_distance = distance;
	}

	vec perpendicular(position.dir().y, -position.dir().x);
	vec goal_dir = path.get_direction(position.pos(), position.dir());

	float proj = dot(perpendicular, goal_dir);
DEBUG(Serial << "dist: " << distance << " goal: " << goal_dir << " curr: " << position.dir() << " pos: " << position.pos());

	if (dot(position.dir(), goal_dir) > 0)
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
DEBUG(Serial << "   speed: " << speed);
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

DEBUG(Serial << endl);
	return true;
}