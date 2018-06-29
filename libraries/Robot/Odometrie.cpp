#include <Odometrie.h>
#include <math.h>


void Odometrie::setup(Odometrie::Config config)
{
	mode = config.mode;
	if (mode == DOUBLE_CODEUSE)
	{
		codeuses[GAUCHE].setup(config.gauche);
		codeuses[DROITE].setup(config.droite);

		ecart_entre_roues = config.ecart_entre_roues;
	}
	else
	{
		codeuse.setup(config.codeuse);
		gyro.setup();
	}

	x = y = 0;
	alpha = 0;
	Lprecedent = 0;
}

void Odometrie::update()
{
	if (mode == DOUBLE_CODEUSE)
		updateDoubleCodeuse();
	else
		updateCodeuseGyroscope();
}

void Odometrie::updateDoubleCodeuse()
{
	float Lg = codeuses[GAUCHE].getDistance(), Ld = codeuses[DROITE].getDistance();

	alpha = -(Lg + Ld) / ecart_entre_roues;

	float L = (-Lg + Ld) * 0.5f;
	float deltaL = L - Lprecedent;
	Lprecedent = L;

	float radians = alpha * (M_PI / 180.0);
	x += deltaL * cos(radians);
	y += deltaL * sin(radians);
}

void Odometrie::updateCodeuseGyroscope()
{
	alpha = gyro.getAlpha();

	float L = codeuse.getDistance();
	float deltaL = L - Lprecedent;
	Lprecedent = L;

	float radians = alpha * (M_PI / 180.0);
	x += deltaL * cos(radians);
	y += deltaL * sin(radians);
}

float Odometrie::getX()
{
	return x;
}

float Odometrie::getY()
{
	return y;
}

float Odometrie::getAlpha()
{
	return alpha;
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
		float g = gyro.getAlpha();
		float e = ; // Ecart entre les roues

		if (num == 0)
			return -c - 0.5f*e*g;

		else
			return c - 0.5f*e*g;
	}
}