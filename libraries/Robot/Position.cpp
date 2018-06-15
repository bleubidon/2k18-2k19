#include <PositionDuPetitRobot.h>
#include <math.h>

Position::Position() :
	x(0), y(0), alpha(0)
	Lprecedent(0)
{ }

void Position::update(Encodeur* codeuse, Gyroscope* gyro)
{
	alpha = gyro->getAlpha();

	float L = codeuse->getDistance();
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
