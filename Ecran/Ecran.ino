#include <I2CParser.h>
#include <TFTv2.h>
#include <SeeedTouchScreen.h>

// Serial print helpers
template <class T> inline Print &operator<<(Print &obj, T arg) { obj.print(arg); return obj; }
const char endl = '\n';

#define ADDRESS 42
#define PERIODE 500 // millisecondes

// SCREEN MODES
#define MENU 0
#define COULEUR 1
#define TIMER 2
#define GYRO 3

void couleur(), timer();
void (*modes[])() = {NULL, couleur, timer, NULL};

I2CParser parser;
Point click;
int mode, x;

void setup()
{
	Serial.begin(9600);

	TFT_BL_ON;	 // turn on the background light
	Tft.TFTinit(); // init TFT library

	parser.add("menu", setup_menu);
	parser.add("couleur", setup_couleur);
	parser.add("timer", setup_timer);
	parser.add("gyro", setup_gyro);

	parser.setup(ADDRESS);
	setup_menu();
}

void loop()
{
	parser.loop();

	if (updateDisplay(PERIODE) && modes[mode] != NULL)
		(*modes[mode])();
}

/*** MODES ***/

// Menu
void setup_menu()
{
	mode = MENU;

	Tft.fillScreen();
	Tft.drawString("Menu", 70, 20, 4, WHITE);
}

// Couleur
void setup_couleur(int argc, char **argv)
{
	mode = COULEUR;

	INT16U c1 = BLUE, c2 = RED;
	if (argc == 3)
	{
		c1 = atoi(argv[1]);
		c2 = atoi(argv[2]);
	}

	Tft.fillRectangle(MIN_X, MIN_Y, MAX_X, 160, c1);
	Tft.fillRectangle(MIN_X, 160, MAX_X, 160, c2);
}

void couleur()
{
	if (getPoint(&click))
	{
		if (click.y < 160)
			parser.setAnswer("0");
		else
			parser.setAnswer("1");

		setup_menu();
	}
}

// Timer
void setup_timer()
{
	mode = TIMER;
	x = 0;

	Tft.fillScreen();
	Tft.drawString("Timer", 60, 20, 4, WHITE);
	Tft.drawNumber(x, 90, 70, 4, RED);
}

void timer()
{
	static unsigned long lastUpdate = 0;
	unsigned long current = millis();

	if (current - lastUpdate < 1000)
		return;

	lastUpdate = current;

	Tft.fillRectangle(0, 60, MAX_X, 28, BLACK);
	Tft.drawNumber(++x, 90, 70, 4, RED);
}

// Gyro
void setup_gyro(int argc, char **argv)
{
	if (mode != GYRO)
	{
		mode = GYRO;

		Tft.fillScreen();
		Tft.drawString("Gyro", 60, 20, 4, WHITE);
	}
	else
		Tft.fillRectangle(0, 70, MAX_X, 28, BLACK);

	Serial << argv[1] << endl;
	Tft.drawNumber(atoi(argv[1]), 90, 70, 4, RED);
}


/*** HELPERS ***/

bool updateDisplay(unsigned long periode)
{
	static unsigned long lastUpdate = 0;
	unsigned long current = millis();

	if (current - lastUpdate < periode)
		return false;

	lastUpdate = current;

	return true;
}

bool getPoint(Point *p)
{
	static TouchScreen ts = TouchScreen(XP, YP, XM, YM);

	*p = ts.getPoint();
	p->x = map(p->x, TS_MINX, TS_MAXX, MIN_X, MAX_X);
	p->y = map(p->y, TS_MINY, TS_MAXY, MIN_Y, MAX_Y);

	return p->z > __PRESSURE;
}
