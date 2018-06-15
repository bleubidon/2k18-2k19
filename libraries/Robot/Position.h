#pragma once

#include "Encodeur.h"
#include "Gyroscope.h"

class Position
{

	public:
		Position();

		void update(const Encodeur& gauche, const Encodeur& droit);
		void update(const Encodeur& codeuse, const Gyroscope& gyro);
		
		float getX();
		float getY();
		float getAlpha();
	
	private:
		float x, y;
		float alpha;

		float Lprecedent;
};