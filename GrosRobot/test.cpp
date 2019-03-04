#include "test.h"

#ifdef TEST

#define DEFINE_TEST(str) void test_ ##str (int argc, char **argv)
#define EXEC_TEST(str)	 test_ ##str (argc - 2, argv + 2)

#define UNIT_TEST(str) \
	if (argc == 2 || !strcmp(argv[1], #str)) \
		EXEC_TEST(str)

#define WAIT_CONSIGNE(cmd) do {\
		#cmd ;\
		while (!Robot.loop_pid()) \
			; \
	} while (0)

/// TESTS
DEFINE_TEST(rot)
{
	WAIT_CONSIGNE(Robot.consigne(0.f, 45.f));
	WAIT_CONSIGNE(Robot.consigne(0.f, -45.f));
	WAIT_CONSIGNE(Robot.consigne(0.f, 0.f));
}

DEFINE_TEST(dist)
{
	WAIT_CONSIGNE(Robot.consigne_rel(10.f, 0.f));
	WAIT_CONSIGNE(Robot.consigne_rel(-10.f, 0.f));
}

/// ALIAS
DEFINE_TEST(asserv)
{
	EXEC_TEST(rot);
	EXEC_TEST(dist);
}

/// INTERFACE
void unit_test(int argc, char **argv)
{
	UNIT_TEST(dist);
	UNIT_TEST(rot);
}

#endif
