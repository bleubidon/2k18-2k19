#include "Actions.h"
#include <LiquidCrystal.h>

const int LCD_length = 16;
LiquidCrystal lcd(33,31,29,27,25,23);
char default_text[] = "AIR 2K19";

void setup_ecran()
{
	lcd.begin(16, 2);
	clear_ecran();
}

void clear_ecran()
{
	lcd.clear();
	affichage(default_text, 0);
}

void affichage(const char *message, unsigned ligne, bool clear_display)
{
	DEBUG(Serial << "LCD: " << message << endl);

	if (ligne > 1)
		ligne = 1;
	if (clear_display)
		clear_ecran();

	int message_length = strlen(message);
	int cursor = 0;
	if (message_length < LCD_length)
		cursor = (message_length % 2 == 0) ?
			LCD_length / 2 - message_length / 2:
			LCD_length / 2 - (message_length + 1) / 2;

	lcd.setCursor(0, ligne);
	lcd.print("                ");
	lcd.setCursor(cursor, ligne);
	lcd.print(message);
}