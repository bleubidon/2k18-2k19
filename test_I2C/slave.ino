#include <Wire.h>

#include <stdint.h>
#include <TFTv2.h>
#include <SPI.h>
#include <stdint.h>
#include <SeeedTouchScreen.h>

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // mega
#define YP A2												   // must be an analog pin, use "An" notation!
#define XM A1												   // must be an analog pin, use "An" notation!
#define YM 54												   // can be a digital pin, this is A0
#define XP 57												   // can be a digital pin, this is A3

#elif defined(__AVR_ATmega32U4__) // leonardo
#define YP A2					  // must be an analog pin, use "An" notation!
#define XM A1					  // must be an analog pin, use "An" notation!
#define YM 18					  // can be a digital pin, this is A0
#define XP 21					  // can be a digital pin, this is A3

#else		  //168, 328, something else
#define YP A2 // must be an analog pin, use "An" notation!
#define XM A1 // must be an analog pin, use "An" notation!
#define YM 14 // can be a digital pin, this is A0
#define XP 17 // can be a digital pin, this is A3

#endif

#define TS_MIN_X 116 * 2
#define TS_MAX_X 890 * 2
#define TS_MIN_Y 83 * 2
#define TS_MAX_Y 913 * 2

TouchScreen ts = TouchScreen(XP, YP, XM, YM);

// SCREEN MODES
#define MENU	0
#define COULEUR 1
// SCREEN MODES

int mode = MENU;
char response = 0;

const byte ADDRESS = 42;
const unsigned long displayDelay = 100;


void setup_slave()
{
	Wire.begin(ADDRESS);
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent);

	TFT_BL_ON;	 // turn on the background light
	Tft.TFTinit(); // init TFT library
}

void loop_slave()
{
	updateDisplay();
}



/******   COMMUNICATION   ******/

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
	Wire.write(response);
	Serial << "Answering >" << response << "<   ASCII: " << (int)response << endl;
}


/******   DISPLAY   ******/

void updateDisplay()
{
	static unsigned long lastUpdate = 0;
	unsigned long current = millis();

	if (current - lastUpdate < displayDelay)
		return;

	lastUpdate = current;

	switch (mode)
	{
		case MENU:
			break;

		case COULEUR:
			loop_couleur();
			break;
	}
}

void setup_couleur()
{
	mode = COULEUR;
	response = 0;
	
	Tft.fillRectangle(0, 0, 250, 160, BLUE);
	Tft.fillRectangle(0, 160, 250, 160, RED);
}

void loop_couleur()
{
	Point p = ts.getPoint();
	p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
	p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);

	if (p.z > __PRESSURE)
	{
		if (p.y > 160)
			response = '0';

		else
			response = '1';
	}
}
