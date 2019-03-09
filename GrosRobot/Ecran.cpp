#include "Actions.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(49, 47, 45, 43, 42, 40);

void setup_ecran()
{
	lcd.begin(16, 2);

	lcd.clear();

	lcd.setCursor(0, 0);
	lcd.print("      AIR       ");

	lcd.setCursor(0, 1);
	lcd.print("      2k19      ");
}
