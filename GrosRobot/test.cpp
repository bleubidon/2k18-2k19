#include "test.h"

#ifdef TEST

#include "Actions.h"

#define DEFINE_TEST(str) void test_ ##str (int argc, char **argv)
#define EXEC_TEST(str)	 test_ ##str (argc - 2, argv + 2)

#define UNIT_TEST(str) \
	if (argc == 1 || !strcmp(argv[1], #str)) \
	{	EXEC_TEST(str); tested = true;	}

#define WAIT_CONSIGNE(cmd) do {\
		#cmd ;\
		while (!Robot.loop_pid()) \
			; \
	} while (0)

DEFINE_TEST(dist)
{
	WAIT_CONSIGNE(Robot.consigne_rel(10.f, 0.f));
	WAIT_CONSIGNE(Robot.consigne_rel(-10.f, 0.f));
}

DEFINE_TEST(rot)
{
	WAIT_CONSIGNE(Robot.consigne(0.f, 45.f));
	WAIT_CONSIGNE(Robot.consigne(0.f, -45.f));
	WAIT_CONSIGNE(Robot.consigne(0.f, 0.f));
}

static void test_capteur_butee(int pin)
{
	while (digitalRead(pin) == HIGH)
		;

	DEBUG(Serial << "Butee ON" << endl);
	delay(100);

	while (digitalRead(pin) == LOW)
		;

	DEBUG(Serial << "Butee OFF" << endl);
}

DEFINE_TEST(butee)
{
	DEBUG(Serial << "Test de butees" << endl);
  affichage(" Test de butees ");
  delay(1000);
  affichage(" Test butee bas ");
	DEBUG(Serial << "1- Bas" << endl);
	test_capteur_butee(28);
  affichage("  Butee Bas OK  ");
  delay(1000);
  affichage("Test butee haut ");
	DEBUG(Serial << "2- Haut" << endl);
	test_capteur_butee(22);
  affichage("  Butee Haut OK ");
  delay(1000);
  affichage("Test butee palet");
	DEBUG(Serial << "3- Palet" << endl);
	test_capteur_butee(30);
  affichage(" Butees Palet OK ");
}

DEFINE_TEST(plateau)
{
  DEBUG(Serial << "Test plateau" << endl);
  affichage("  Test plateau  ");
  delay(1000);
  affichage("Descente plateau");
  DEBUG(Serial << "Descente plateau" << endl);
	descente_plateau();
  affichage("  Descente OK!  ");
	delay(500);
 affichage(" Montee plateau ");
 DEBUG(Serial << "Montée plateau" << endl);
	montee_plateau();
 affichage("   Montee OK!   ");
}

/// INTERFACE
void unit_test(int argc, char **argv)
{
	bool tested = false; // may be set to true by UNIT_TEST

	UNIT_TEST(dist);
	UNIT_TEST(rot);
	UNIT_TEST(butee);
	UNIT_TEST(plateau);

	if (!tested)
		Serial << "This test doesn't exist" << endl;
}

#endif
