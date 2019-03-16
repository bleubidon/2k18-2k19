#include "Actions.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(33,31,29,27,25,23);

void setup_ecran()
{
	lcd.begin(16, 2);
	lcd.clear();
	lcd.setCursor(4,0);
	lcd.print("AIR 2K19");
}

void clear_ecran()
{
	lcd.clear();
	lcd.setCursor(4,0);
	lcd.print("AIR 2K19");
	lcd.setCursor(0,1);
	lcd.print("                ");
}

void affichage(const char * message)
{
	lcd.setCursor(0,1);
	lcd.print(message);
	delay(250);
}
