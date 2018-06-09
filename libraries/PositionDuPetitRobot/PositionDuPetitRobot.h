#ifndef _PositionDuPetitRobot_H_
#define _PositionDuPetitRobot_H_

class PositionDuPetitRobot {

public :
PositionDuPetitRobot(double xInit = 0, double yInit = 0, float alphaInit = 0);
void majPosition(long cntr, float gyro);
double getX();
double getY();
double getAlpha();
double setY(int a);
double setX(int a);
double setAlpha(float a);

private :

double x;//100*toPasCodeuse;
double y;//1000*toPasCodeuse;
double alpha;
double alphaIni;
double L;
double Lprecedent;
double deltaL;

};

#endif