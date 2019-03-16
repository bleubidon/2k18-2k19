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

void clear_ecran();
void affichage(const char* message);
