#pragma once

#include <Robot.h>

extern TaskQueue do_square;

void setup_ecran();
void setup_actions();
void setup_ascenseur();

void loop_actions();

void descente_plateau();
void montee_plateau();

void cycle_ascenseur();

void affichage(char* message);
void showLetters(int printStart, int startLetter, char* message);
