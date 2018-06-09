#include <PositionDuPetitRobot.h>
#include <math.h>

PositionDuPetitRobot::PositionDuPetitRobot(double xInit, double yInit, float alphaInit):
	x(xInit), y(yInit),
	alpha(alphaInit), alphaIni(alphaInit),
	L(0), Lprecedent(0), deltaL(0)
{}

void PositionDuPetitRobot::majPosition(long cntr, float gyro)
{
	//float toDegree=17.4072f;// DIAMETRE_ENTRE_ROUE*PIE*NB_PAS/(DIAMETRE_ROUE_CODEUSE*PIE)/360*2
	L = cntr; //moyenne des codeuses codeuses
	//alpha = -(cntr2+cntr1)+alphaIni; //coef pour passer de la différence des codeuses à un angle en degrée = (distanceEntreRoues*nbPas)/(180*diametreRoueCodeuse)=27380.95
	deltaL = L - Lprecedent;
	//deltaL = deltaL/5.3053;
	alpha = gyro;
	Lprecedent = L;
	double deltaY = deltaL * cos(alpha * (M_PI / 180));
	double deltaX = deltaL * sin(alpha * (M_PI / 180));
	x += deltaX;
	y += deltaY;
}

double PositionDuPetitRobot::getX()
{
	return x / 5.3053; //renvoie la distance à l'origine de x NB_PAS/(DIAMETRE_ROUE_CODEUSE*PI) == 800/(48 * pi) =~ 5.3053
}

double PositionDuPetitRobot::getY()
{
	return y / 5.3053; //renvoie la distance à l'origine de y NB_PAS/(DIAMETRE_ROUE_CODEUSE*PI) == 800/(48 * pi) =~ 5.3053
}

double PositionDuPetitRobot::getAlpha()
{
	return alpha;
}

double PositionDuPetitRobot::setX(int a)
{
	x = a * 5.3053;
	return x;
}

double PositionDuPetitRobot::setY(int a)
{
	y = a * 5.3053;
	return y;
}

double PositionDuPetitRobot::setAlpha(float a)
{
	alphaIni = 0;
	return alpha;
}
