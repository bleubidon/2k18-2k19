#include <Odometrie.h>
#include <math.h>


void Odometrie::setup(Odometrie::Config config)
{
	mode = config.mode;
	if (mode == DOUBLE_CODEUSE)
	{
		gauche.setup(config.codeuses[0]);
		droite.setup(config.codeuses[1]);

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
	float Lg = gauche.getDistance(), Ld = droite.getDistance();

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
