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

	num_sicks = config.num_sicks;
	capteurs = new Sick[num_sicks];
	//capteurs = (Sick*)malloc(num_sicks * sizeof(Sick));
	for (int i(0); i < num_sicks; i++)
		capteurs[i].setup(config.sicks[i]);

	min_speed = config.min_speed;
	max_speed = config.max_speed;

	duration = config.dureeMatch;

	dist = config.dist;
	rot = config.rot;
	consigne_pid = false;
}

void c_Robot::start()
{
	start_time = micros();

	// Setup timer interrupt
		cli(); // Disable interrupts

		// Set timer1 interrupt at 1Hz
		TCCR1A = 0; // set entire TCCR1A register to 0
		TCCR1B = 0; // same for TCCR1B
		TCNT1  = 0; // initialize counter value to 0

		// Set compare match register for 1hz increments
		OCR1A = 15624; // = (16*10^6) / (1*1024) - 1 (must be <65536)
		// turn on CTC mode
		TCCR1B |= (1 << WGM12);
		// Set CS12 and CS10 bits for 1024 prescaler
		TCCR1B |= (1 << CS12) | (1 << CS10);
		// enable timer compare interrupt
		TIMSK1 |= (1 << OCIE1A);

		sei(); // Enable interrupts
}

void c_Robot::stop()
{
	moteurs[GAUCHE].consigne(0);
	moteurs[DROITE].consigne(0);
}

void c_Robot::consigne(float _dist, float _rot)
{
    Serial << "consigne: ";
    Serial <<_dist << endl;

	dist.set_consigne(_dist);
	rot.set_consigne(_rot);

	prev_time = millis();
	consigne_pid = true;

	LOG(Serial << "CMD: " << _dist << "," << _rot << endl);
}

void c_Robot::consigne_rel(float _dist, float _rot)
{
	consigne((position.dist() + _dist), position.rot() + _rot);
}

// Helpers
void c_Robot::translate(float _dist, bool blocking)
{
	consigne_rel(_dist, 0);

	if (blocking)
		while (Robot.loop_pid())
			;
}

void c_Robot::rotate(float _angle, bool blocking)
{
	consigne_rel(0, _angle);

	if (blocking)
		while (Robot.loop_pid())
			;
}

void c_Robot::go_to(vec _dest, bool blocking)
{
	// Rotate toward destination (blocking)
	look_at(_dest, true);

	// Move forward
	vec dir = _dest - position.pos();
	consigne_rel(vec::dist(position.pos(), _dest), dir.angle() - position.rot());
    
	if (blocking)
		while (Robot.loop_pid())
			;
}

void c_Robot::go_to_bkwd(vec _dest, bool blocking)
{
    // Rotate toward destination (blocking)
//    look_at(_dest, true);

    // Move backward
    vec dir = _dest - position.pos();

    Serial << "current angle: " << position.rot() << endl;

    consigne_rel(-vec::dist(position.pos(), _dest), 0); // dir.angle() - position.rot() + 180

    if (blocking)
        while (Robot.loop_pid())
            ;
}

void c_Robot::look_at(vec _point, bool blocking)
{
	vec dir = _point - position.pos();
	consigne(position.dist(), dir.angle());

	if (blocking)
		while (Robot.loop_pid())
			;
}

// timer1 interrupt 1Hz
ISR(TIMER1_COMPA_vect)
{
	unsigned long now = micros();

	if (now - Robot.start_time > Robot.duration)
	{
		while (1)
			;
	}
}


// PID
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
		return speed + min_speed;
//		return min(speed + min_speed, max_speed);
	else
		return speed - min_speed;
//		return max(speed - min_speed, -max_speed);
}

bool c_Robot::loop_pid()
{
    Serial << consigne_pid << endl;
	if (!consigne_pid)
		return false;

	unsigned long stop_start = millis(); 
	int i = 0;
	while (i < num_sicks)
	{

		if (capteurs[i++].is_active())
		{
			Serial << "STOOOOOP" << i << endl;
			stop();
			i = 0;
		}
	}
	


	// Mise a jour de la consigne toute les 10 millisecondes (au moins)
	const unsigned long min_delay = 10;
	unsigned long current_time = millis();
	prev_time += (current_time - stop_start);

	unsigned long dt = current_time - prev_time;
	if (dt < min_delay)
		return true;
	prev_time = current_time;


	// Lecture des capteurs de position et calcul de la consigne grace aux PIDs
	position.update();

	float vitesse_dist = dist.compute(dist.consigne - position.dist(), dt);
	float vitesse_rot = rot.compute(angle_diff(rot.consigne, position.rot()), dt);

	const float precision_dist = 0.5f;
	const float precision_rot = 0.5f; //0.5f
	if (abs(dist.erreur) < precision_dist && abs(rot.erreur) < precision_rot)
	{
		stop();
		consigne_pid = false;
		Serial << position.rot() << "   " << position.dist() << endl;

		return false;
	}
	else
	{
		// Ecretage des accelerations
		static float dist_vitesse_old = 0;
		static float rot_vitesse_old = 0;
		const float dvMax_dist = 20;
		const float dvMax_rot = 10;
		vitesse_dist = clamp(-dvMax_dist, (vitesse_dist - dist_vitesse_old), dvMax_dist)  + dist_vitesse_old;
		vitesse_rot = clamp(-dvMax_rot, (vitesse_rot - rot_vitesse_old), dvMax_rot)  + rot_vitesse_old;

        float vg = scale(vitesse_dist + vitesse_rot);
        float vd = scale(vitesse_dist - vitesse_rot);

        if (vg >= max_speed || vd >= max_speed)
        {
            float ratio = max_speed / max(vg, vd);
            vg *= ratio;
            vd *= ratio;
        }
        else if (vg <= -max_speed || vd <= -max_speed)
        {
            float ratio = -max_speed / min(vg, vd);
            vg *= ratio;
            vd *= ratio;
        }

		moteurs[0].consigne(vg);
		moteurs[1].consigne(vd);

		dist_vitesse_old = vitesse_dist;
		rot_vitesse_old = vitesse_rot;
	}

	// Ecrit les donnees de log sur le port serie
	LOG(Serial << "PID:" << current_time << "," <<
		dist.consigne << "," << position.dist() << "," <<
		rot.consigne << "," << position.rot()
		<< "    " << vitesse_dist << "  " << vitesse_rot
		 << endl;)
	return true;
}
