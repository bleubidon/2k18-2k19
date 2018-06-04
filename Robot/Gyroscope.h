#pragma once

#include <Adafruit_BNO055.h>

#define BRAKEVCC 0
#define CW   1
#define CCW  2
#define BRAKEGND 3

class Gyroscope
{
	public:
		Gyroscope();

		bool setup();

		float getOrientation();

	private:
		Adafruit_BNO055 bno;
};