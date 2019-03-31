#include "Actions.h"
#include <LiquidCrystal.h>

const int pinBouton = 49;
LiquidCrystal lcd(33,31,29,27,25,23);

void setup_ecran()
{
	lcd.begin(16, 2);
	lcd.clear();
	lcd.setCursor(4,0);
	lcd.print("AIR 2K19");

	pinMode(pinBouton, INPUT_PULLUP);
}

void clear_ecran()
{
	lcd.clear();
	lcd.setCursor(4,0);
	lcd.print("AIR 2K19");
	lcd.setCursor(0,1);
	lcd.print("                ");
}

void affichage(const char *message, unsigned ligne)
{
	DEBUG(Serial << message << endl);

	lcd.setCursor(0, ligne > 1 ? 1 : ligne);
	lcd.print(message);
	delay(250);
}
