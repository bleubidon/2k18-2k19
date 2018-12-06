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
		Serial << "Setup codeuses..." << endl;
		codeuses[GAUCHE].setup(config.gauche);
		codeuses[DROITE].setup(config.droite);

		ecart_entre_roues = config.ecart_entre_roues;
		break;
	
	case CODEUSE_GYROSCOPE:
		Serial << "Setup codeuse..." << endl;
		codeuse.setup(config.codeuse);
		Serial << "Setup gyro..." << endl;
		gyro.setup();
		break;
	}

	position.set(0, 0);
	direction.set(0, 0);
	angle = 0;
	Lprecedent = 0;
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
	angle = rad * (360 / TWO_PI);

	float L = (Lg + Ld) * 0.5f;
	float dL = L - Lprecedent;
	Lprecedent = L;

	direction.set( cos(rad), sin(rad) );
	position += dL * direction;
}

void Odometrie::updateCodeuseGyroscope()
{
	angle = gyro.rot();

	float L = codeuse.getDistance();
	float dL = L - Lprecedent;
	Lprecedent = L;

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

const float& Odometrie::rot()
{
	return angle;
}

float Odometrie::getPositionCodeuse(int num)
{
	if (mode == DOUBLE_CODEUSE)
	{
		return codeuses[num].getDistance();
	}
	else
	{
		float c = codeuse.getDistance();
		float g = gyro.rot();
		float e = 20.0f; // Ecart entre les roues

		if (num == 0)
			return -c - 0.5f*e*g;

		else
			return c - 0.5f*e*g;
	}
}