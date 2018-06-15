#include <Position.h>
#include <math.h>

Position::Position() :
	x(0), y(0), alpha(0),
	Lprecedent(0)
{ }

void Position::update(const Encodeur& gauche, const Encodeur& droit)
{
	static const float DISTANCE_ENTRE_ROUES = 275.0f;
	
	float Lg = gauche.getDistance(), Ld = droit.getDistance();

	alpha = -(Lg + Ld) / DISTANCE_ENTRE_ROUES;

	float L = (-Lg + Ld) * 0.5f;
	float deltaL = L - Lprecedent;
	Lprecedent = L;

	float radians = alpha * (M_PI / 180.0);
	x += deltaL * cos(radians);
	y += deltaL * sin(radians);
}

void Position::update(const Encodeur& codeuse, const Gyroscope& gyro)
{
	alpha = gyro.getAlpha();

	float L = codeuse.getDistance();
	float deltaL = L - Lprecedent;
	Lprecedent = L;

	float radians = alpha * (M_PI / 180.0);
	x += deltaL * cos(radians);
	y += deltaL * sin(radians);
}

float Position::getX()
{
	return x;
}

float Position::getY()
{
	return y;
}

float Position::getAlpha()
{
	return alpha;
}
