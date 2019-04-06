#include "test.h"

#ifdef TEST

#include "Actions.h"

#define DEFINE_TEST(str) void test_ ##str (int argc, char **argv)
#define EXEC_TEST(str)	 test_ ##str (argc - 2, argv + 2)

#define UNIT_TEST(str) \
	if (argc == 1 || !strcmp(argv[1], #str)) \
	{	EXEC_TEST(str); tested = true;	}

#define WAIT_CONSIGNE(cmd) do {\
		cmd ;\
		while (Robot.loop_pid()) \
			; \
	} while (0)

DEFINE_TEST(dist)
{
	affichage("Deplacement", 0);
	delay(1000);

	WAIT_CONSIGNE(Robot.consigne_rel(10.f, 0.f));
	WAIT_CONSIGNE(Robot.consigne_rel(-10.f, 0.f));
	clear_ecran();
}

DEFINE_TEST(rot)
{
	affichage("Rotation", 0);
	delay(1000);

	WAIT_CONSIGNE(Robot.consigne(0.f, 45.f));
	WAIT_CONSIGNE(Robot.consigne(0.f, -45.f));
	WAIT_CONSIGNE(Robot.consigne(0.f, 0.f));
	clear_ecran();
}

static void test_capteur_butee(int pin)
{
	while (digitalRead(pin) == HIGH)
		;

	delay(100);

	while (digitalRead(pin) == LOW)
		;
}

DEFINE_TEST(butee)
{
	affichage("Test de butees", 0);
	delay(1000);

	affichage("Test butee basse");
	test_capteur_butee(28);
	affichage("Butee basse OK");
	delay(1000);

	affichage("Test butee haute");
	test_capteur_butee(22);
	affichage("Butee haute OK");
	delay(1000);

	affichage("Test butee palet");
	test_capteur_butee(30);
	affichage("Butee palet OK");
	delay(1000);

	clear_ecran();
}

DEFINE_TEST(plateau)
{
	affichage("Test plateau", 0);

	affichage("Descente plateau");
	descente_plateau();
	affichage("Descente OK!");
	delay(500);

	affichage("Montee plateau");
	montee_plateau();
	affichage("Montee OK!");
	delay(500);

	clear_ecran();
}

DEFINE_TEST(pinces)
{
	affichage("Test pinces", 0);
	delay(1000);

	affichage("Ouverture");
	set_pinces(opened_pliers_values[GAUCHE], opened_pliers_values[DROITE]);
	delay(500);

	affichage("Fermeture");
	set_pinces(closed_pliers_values[GAUCHE], closed_pliers_values[DROITE]);
	delay(500);

	clear_ecran();
}

/// INTERFACE
void unit_test(int argc, char **argv)
{
	bool tested = false; // may be set to true by UNIT_TEST

	UNIT_TEST(dist);
	UNIT_TEST(rot);
	UNIT_TEST(butee);
	UNIT_TEST(plateau);
	UNIT_TEST(pinces);

	if (!tested)
		Serial << "This test doesn't exist" << endl;
}

#endif
