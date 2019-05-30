#include <Odometrie.h>
#include <math.h>

const uint8_t DOUBLE_CODEUSE = 0;
const uint8_t CODEUSE_GYROSCOPE = 1;

void Odometrie::setup(Odometrie::Config config)
{
	mode = config.mode;

	switch (mode)
	{
	case DOUBLE_CODEUSE:
		codeuses[GAUCHE].setup(config.gauche);
		codeuses[DROITE].setup(config.droite);

		ecart_entre_roues = config.ecart_entre_roues;
		break;

	case CODEUSE_GYROSCOPE:
		codeuse.setup(config.codeuse);
		gyro.setup();
		break;
	}

	position.set(config.position.x, config.position.y);
	angle = config.angle;

	direction = vec(angle);
	distance = 0;
	dist_prev = 0;
}

void Odometrie::update()
{
	switch (mode)
	{
	case DOUBLE_CODEUSE:
		updateDoubleCodeuse();
		break;

	case CODEUSE_GYROSCOPE:
		updateCodeuseGyroscope();
		break;
	}
}

void Odometrie::updateDoubleCodeuse()
{
	float Lg = -codeuses[GAUCHE].getDistance(), Ld = codeuses[DROITE].getDistance();

	float rad = (Lg - Ld) / ecart_entre_roues;
	angle = fmod(rad * (360 / TWO_PI), 360);

	distance = (Lg + Ld) * 0.5f;
	float dL = distance - dist_prev;
	dist_prev = distance;

	direction.set( cos(rad), sin(rad) );
	position += dL * direction;

	//Serial << Lg <<"   " << Ld << endl;
}

void Odometrie::updateCodeuseGyroscope()
{
	angle = 360 - gyro.rot(); // Inversion car il est a l'envers sur le robot :(

	distance = codeuse.getDistance();
	float dL = distance - dist_prev;
	dist_prev = distance;

	float rad = radians(angle);
	direction.set( cos(rad), sin(rad) );
	position += dL * direction;
}

const vec& Odometrie::pos()
{
	return position;
}

const vec& Odometrie::dir()
{
	return direction;
}

const float& Odometrie::dist()
{
	return distance;
}

const float& Odometrie::rot()
{
	return angle;
}
