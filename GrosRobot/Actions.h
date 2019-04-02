#pragma once

#include <Robot.h>

extern TaskQueue do_square;

void setup_ecran();
void setup_actions();
void setup_ascenseur();

void loop_actions();

void descente_plateau();
void montee_plateau();
void set_pinces(int gauche, int droite);

void cycle_ascenseur();

void clear_ecran();
void affichage(const char* message, unsigned ligne = 1, bool clear_display=false);

extern const int opened_pliers_values[2];
extern const int closed_pliers_values[2]; 
