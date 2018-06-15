#pragma once

#include "Encodeur.h"
#include "Gyroscope.h"

class Position
{

  public:
	Position();

	void update(Encodeur* codeuse, Gyroscope* gyro);
	
	float getX();
	float getY();
	float getAlpha();
	

  private:
	float x, y;
	float alpha;

	float Lprecedent;
};