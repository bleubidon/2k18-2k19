#pragma once

#include <Wire.h>
#include <TFTv2.h>
#include <SeeedTouchScreen.h>


// Serial print helpers
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
const char endl = '\n';


void receiveEvent(int length)
{
	Serial << "Received " << length << " bytes from master" << endl;
	while (Wire.available())
	{
		char c = Wire.read();
		Serial << ">" << c << "<   ASCII: " << (int)c << endl;
	}

	Serial << endl;
}

void requestEvent()
{
	char response = 0;
	Wire.write(response);
	Serial << "Answering >" << response << "<   ASCII: " << (int)response << endl;
}

void setup_ecran(byte address)
{
	Wire.begin(address);
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent);

	TFT_BL_ON;	 // turn on the background light
	Tft.TFTinit(); // init TFT library
}


bool updateDisplay(unsigned long periode)
{
	static unsigned long lastUpdate = 0;
	unsigned long current = millis();

	if (current - lastUpdate < periode)
		return false;

	lastUpdate = current;

	return true;
}

bool getPoint(Point* p)
{
	static TouchScreen ts = TouchScreen(XP, YP, XM, YM);

	*p = ts.getPoint();
	p->x = map(p->x, TS_MINX, TS_MAXX, MIN_X, MAX_X);
	p->y = map(p->y, TS_MINY, TS_MAXY, MIN_Y, MAX_Y);

	return p->z > __PRESSURE;
}