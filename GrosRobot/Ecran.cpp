#include "Actions.h"
#include <LiquidCrystal.h>

const int LCD_length = 16;
const int pinBouton = 49;
LiquidCrystal lcd(33,31,29,27,25,23);
char default_text[] = "AIR 2K19";

void setup_ecran()
{
	lcd.begin(16, 2);
    pinMode(pinBouton, INPUT_PULLUP);
    clear_ecran();
}

void clear_ecran()
{
	lcd.clear();
	affichage(default_text, 0);
}

void affichage(const char *message, unsigned ligne, bool clear_display=false) // ligne = 1 par defaut, cf Actions.h
{
    if (clear_display) clear_ecran();
    DEBUG(Serial << "Write on LCD: " << message << endl);
    int message_length = strlen(message);
    int cursor = message_length > LCD_length ? 0: (message_length % 2 == 0 ? LCD_length / 2 - message_length / 2: LCD_length / 2 - (message_length + 1) / 2);
    lcd.setCursor(cursor, ligne > 1 ? 1: ligne);
	lcd.print(message);
	delay(250);
}
