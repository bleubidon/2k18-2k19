#pragma once

#include <DynamixelSerial2.h>
#include <Parser.h>
#include <Robot.h>

#include <helpers.h>
#include <Button.h>


// Synchronisation vars for chaos zone
#define NO_MORE_ATOM 0
#define MORE_ATOM 1
#define WAIT_RASP 2

// JAUNE = 0 : VIOLET = 1
#define POS_SYM(x, y) (!equipe ? vec(x - 75, y - 25) :  vec(x - 75, - y + 25))
#define ANGLE_SYM(a) (!equipe ? a : -a)

extern Parser parser;
extern byte chaos_zone_state;

extern const int opened_pliers_values[2];
extern const int closed_pliers_values[2];

extern const int pinBas;
extern const int pinHaut;
extern const int pinPalet;


int waitTirette(uint8_t pin, Button& selecteur);
int launch_experience(int equipe);

void setup_ecran();
void setup_parser();
void setup_ascenseur();

void loop_actions();

void descente_plateau();
void montee_plateau();
void set_pinces(int gauche, int droite);

void cycle_ascenseur();
void lacher_palet();

void clear_ecran();
void affichage(const char* message, unsigned ligne = 1, bool clear_display = false);

void demi_cycle();
