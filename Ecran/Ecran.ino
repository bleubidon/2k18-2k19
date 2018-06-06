#include <Parser.h>
#include "definitions.h"

#define ADDRESS 42
#define PERIODE 500 // millisecondes

// SCREEN MODES
#define MENU	0
#define COULEUR 1

void menu(), couleur();
void (*modes[])() = {menu, couleur};


Parser parser;
Point click;
int mode;


void setup()
{
	Serial.begin(9600);

	setup_ecran(ADDRESS);

	parser.add("couleur", setup_couleur);
}

void loop()
{
	parser.loop(); // Note reads serial port for now (must read I2C)

	if (updateDisplay(PERIODE))
		(*modes[mode])();
}


/*** MODES ***/

void setup_menu()
{
	mode = MENU;
	
	Tft.drawString("Menu", 60, 220, 4, WHITE);
}

void menu()
{
}

void setup_couleur(int argc, char** argv)
{
	mode = COULEUR;

	INT16U c1 = BLUE, c2 = RED;
	if (argc == 3)
	{
		c1 = atoi(argv[1]);
		c2 = atoi(argv[2]);
	}
	
	Tft.fillRectangle(0, 0, 250, 160, c1);
	Tft.fillRectangle(0, 160, 250, 160, c2);
}

void couleur()
{
	if (getPoint(&click))
	{
		if (click.y > 160)
			Serial << 'Clicked on first' << endl;
		else
			Serial << 'Clicked on second' << endl;

		setup_menu();
	}
}
