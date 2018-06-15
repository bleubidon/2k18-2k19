#include "Gyroscope.h"
#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno(55);


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

float Gyroscope::getAlpha() const
{
	sensors_event_t event;
	bno.getEvent(&event);

	return event.orientation.x;
}