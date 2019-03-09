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

	DEBUG(Serial << "1- Bas" << endl);
	test_capteur_butee(28);

	DEBUG(Serial << "2- Haut" << endl);
	test_capteur_butee(22);

	DEBUG(Serial << "3- Palet" << endl);
	test_capteur_butee(30);
}

DEFINE_TEST(plateau)
{
	descente_plateau();
	delay(500);
	montee_plateau();
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
