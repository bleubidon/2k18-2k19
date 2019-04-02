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
	affichage("Deplacement");
	delay(1000);

	WAIT_CONSIGNE(Robot.consigne_rel(10.f, 0.f));
	WAIT_CONSIGNE(Robot.consigne_rel(-10.f, 0.f));
	clear_ecran();
}

DEFINE_TEST(rot)
{
	affichage("Rotation");
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
	affichage("Test de butees", 1, true);
	delay(1000);

	affichage("Test butee basse", 1, true);
	test_capteur_butee(28);
	affichage("Butee basse OK", 1, true);
	delay(1000);

	affichage("Test butee haute", 1, true);
	test_capteur_butee(22);
	affichage("Butee haute OK", 1, true);
	delay(1000);

	affichage("Test butee palet", 1, true);
	test_capteur_butee(30);
	affichage("Butee palet OK", 1, true);
	delay(1000);

	clear_ecran();
}

DEFINE_TEST(plateau)
{
	affichage("Test plateau", 1, true);

	affichage("Descente plateau", 1, true);
	descente_plateau();
	affichage("Descente OK!", 1, true);
	delay(500);

	affichage("Montee plateau", 1, true);
	montee_plateau();
	affichage("Montee OK!", 1, true);
	delay(500);

	clear_ecran();
}

DEFINE_TEST(pinces)
{
	affichage("Test pinces", 1, true);
	delay(1000);

	affichage("Ouverture", 1, true);
	set_pinces(opened_pliers_values[GAUCHE], opened_pliers_values[DROITE]);
	delay(500);

	affichage("Fermeture", 1, true);
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
