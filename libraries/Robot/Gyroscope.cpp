#include "Gyroscope.h"
#include "helpers.h"

#include <Adafruit_BNO055.h>

Adafruit_BNO055 bno(55);

bool Gyroscope::setup()
{
	if (!bno.begin())
	{
		LOG(Serial << "GYRO: No BNO055 detected. Check your wiring or I2C ADDR" << endl);
		return false;
	}

	bno.setExtCrystalUse(true);
	return true;
}

float Gyroscope::rot() const
{
	sensors_event_t event;
	bno.getEvent(&event);

	return event.orientation.x;
}
