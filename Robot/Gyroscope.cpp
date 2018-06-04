#include "Gyroscope.h"


Gyroscope::Gyroscope():
	bno(55)
{ }

bool Gyroscope::setup()
{
	if (!bno.begin())
	{
		Serial.println("No BNO055 detected. Check your wiring or I2C ADDR");
		return false;
	}

	bno.setExtCrystalUse(true);
	return true;
}

float Gyroscope::getOrientation()
{
	sensors_event_t event;
	bno.getEvent(&event);

	return event.orientation.x;
}